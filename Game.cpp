// Game.cpp
#include "Game.h"
#include <memory>


Game::Game() : state(GameState::MAP_SELECTION), currentMap(nullptr), selectedSize(0) {
    mapSizes = {
        {12, 12},
        {14, 14},
        {16, 16},
    };
    towerManager = std::make_unique<TowerManager>(1000);

}

Game::~Game() {
    if (currentMap) delete currentMap;
}

void Game::drawTowerMenu() const {
    // Draw bank balance at top
    std::string bankText = "Bank: $" + std::to_string(towerManager->getCurrency());
    DrawText(bankText.c_str(), 10, 10, 30, RED);

    // Tower menu background
    DrawRectangle(0, GetScreenHeight() - towerMenuHeight, GetScreenWidth(), towerMenuHeight, LIGHTGRAY);
    DrawLine(0, GetScreenHeight() - towerMenuHeight, GetScreenWidth(), GetScreenHeight() - towerMenuHeight, BLACK);

    // Tower buttons
    const int buttonWidth = 150;
    const int buttonHeight = 80;
    const int padding = 20;
    const int startY = GetScreenHeight() - buttonHeight - (towerMenuHeight - buttonHeight) / 2;
    const int totalWidth = (buttonWidth + padding) * 3 - padding;
    int startX = (GetScreenWidth() - totalWidth) / 2;

    struct TowerOption {
        std::string type;
        const char* name;
        const char* description;
        int cost;
        Color color;
    };

    TowerOption options[] = {
        {"Basic", "Basic Tower", "DMG: 10\nRange: 150", 100, RED},
        {"Area", "Area Tower", "DMG: 8\nRange: 120", 150, BLUE},
        {"Slow", "Slow Tower", "DMG: 5\nSlow: 30%", 125, YELLOW}
    };

    for (int i = 0; i < 3; i++) {
        Rectangle button = {
            static_cast<float>(startX + i * (buttonWidth + padding)),
            static_cast<float>(startY),
            static_cast<float>(buttonWidth),
            static_cast<float>(buttonHeight)
        };

        // Button background
        Color buttonColor = options[i].color;
        if (selectedTowerType == options[i].type) {
            buttonColor.a = 255;  // Selected tower
        } else if (!towerManager->canAffordTower(options[i].type)) {
            buttonColor.a = 100;  // Can't afford
        } else {
            buttonColor.a = 200;  // Normal state
        }

        DrawRectangleRec(button, buttonColor);
        DrawRectangleLinesEx(button, 2, BLACK);

        // Tower info
        DrawText(options[i].name, button.x + 5, button.y + 5, 20, BLACK);
        DrawText(options[i].description, button.x + 5, button.y + 30, 15, BLACK);

        std::string costText = "$" + std::to_string(options[i].cost);
        DrawText(costText.c_str(), button.x + 5, button.y + buttonHeight - 20, 15, BLACK);

        // Hover effect
        if (CheckCollisionPointRec(GetMousePosition(), button)) {
            DrawRectangleLinesEx(button, 2, WHITE);
        }
    }
}

void Game::handleTowerMenuClick(Vector2 mousePos) {
    if (mousePos.y < GetScreenHeight() - towerMenuHeight) return;

    const int buttonWidth = 150;
    const int buttonHeight = 80;
    const int padding = 20;
    const int startY = GetScreenHeight() - buttonHeight - (towerMenuHeight - buttonHeight) / 2;
    const int totalWidth = (buttonWidth + padding) * 3 - padding;
    int startX = (GetScreenWidth() - totalWidth) / 2;

    for (int i = 0; i < 3; i++) {
        Rectangle button = {
            static_cast<float>(startX + i * (buttonWidth + padding)),
            static_cast<float>(startY),
            static_cast<float>(buttonWidth),
            static_cast<float>(buttonHeight)
        };

        if (CheckCollisionPointRec(mousePos, button)) {
            std::string towerType;
            switch (i) {
                case 0: towerType = "Basic"; break;
                case 1: towerType = "Area"; break;
                case 2: towerType = "Slow"; break;
            }

            if (towerManager->canAffordTower(towerType)) {
                selectedTowerType = (selectedTowerType == towerType) ? "" : towerType;
            }
            break;
        }
    }
}

void Game::initializeMap() {
    if (currentMap) delete currentMap;

    auto [width, height] = mapSizes[selectedSize];
    currentMap = new Map(width, height);
    currentMap->generateRandomMap();
    state = GameState::PLAYING;
}

void Game::handleMapSelection() {
    // Handle keyboard input for size selection
    if (IsKeyPressed(KEY_UP)) {
        selectedSize = (selectedSize - 1 + mapSizes.size()) % mapSizes.size();
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedSize = (selectedSize + 1) % mapSizes.size();
    }
    if (IsKeyPressed(KEY_ENTER)) {
        initializeMap();
    }
}

void Game::update() {
    switch (state) {
        case GameState::MAP_SELECTION:
            handleMapSelection();
        break;

        case GameState::PLAYING: {
            Vector2 mousePos = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (mousePos.y >= GetScreenHeight() - towerMenuHeight) {
                    handleTowerMenuClick(mousePos);
                } else if (!selectedTowerType.empty()) {
                    int cellSize = std::min(GetScreenWidth() / (currentMap->getWidth() + 2),
                                          GetScreenHeight() / (currentMap->getHeight() + 2));
                    int offsetX = (GetScreenWidth() - (currentMap->getWidth() * cellSize)) / 2;
                    int offsetY = (GetScreenHeight() - (currentMap->getHeight() * cellSize)) / 2;

                    int gridX = (mousePos.x - offsetX) / cellSize;
                    int gridY = (mousePos.y - offsetY) / cellSize;

                    if (currentMap->getCellType(gridX, gridY) == CellType::SCENERY) {
                        Vector2 towerPos = {
                            offsetX + gridX * cellSize + cellSize / 2.0f,
                            offsetY + gridY * cellSize + cellSize / 2.0f
                        };

                        if (towerManager->buyTower(selectedTowerType, towerPos)) {
                            currentMap->placeTower(gridX, gridY);
                            selectedTowerType = "";
                        }
                    }
                }
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                selectedTowerType = "";
            }

            updateTowers();
            break;
        }
    }
}

void Game::updateTowers() {
    for (const auto& tower : towerManager->getTowers()) {
        if (tower->canShoot()) {
            // Create shots in 4 directions
            Vector2 start = tower->getPosition();
            float range = tower->getRange();
            Color shotColor = tower->getName() == "Basic Tower" ? RED :
                            tower->getName() == "Area Tower" ? BLUE : YELLOW;

            std::vector<Vector2> directions = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1}
            };

            for (const auto& dir : directions) {
                Vector2 end = {
                    start.x + dir.x * range,
                    start.y + dir.y * range
                };

                activeShots.push_back({start, end, 0.2f, shotColor});  // 0.2s shot duration
            }

            tower->resetShotTimer();
        }
    }

    // Update and remove expired shots
    for (auto it = activeShots.begin(); it != activeShots.end();) {
        it->timer -= GetFrameTime();
        if (it->timer <= 0) {
            it = activeShots.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::drawTowerShots() const {
    for (const auto& shot : activeShots) {
        Color shotColor = shot.color;
        shotColor.a = static_cast<unsigned char>(255 * (shot.timer / 0.2f));
        DrawLineEx(shot.start, shot.end, 2.0f, shotColor);
    }
}

void Game::drawMapSelection() const {
    const char* title = "Select Map Size";
    int fontSize = 30;
    int spacing = 50;

    // Draw title
    Vector2 titlePos = {
        (GetScreenWidth() - MeasureText(title, fontSize)) / 2.0f,
        GetScreenHeight() / 4.0f
    };
    DrawText(title, titlePos.x, titlePos.y, fontSize, BLACK);

    // Draw size options
    for (int i = 0; i < mapSizes.size(); i++) {
        auto [width, height] = mapSizes[i];
        std::string sizeText = std::to_string(width) + "x" + std::to_string(height);

        Color textColor = (i == selectedSize) ? RED : BLACK;
        Vector2 textPos = {
            (GetScreenWidth() - MeasureText(sizeText.c_str(), fontSize)) / 2.0f,
            titlePos.y + spacing * (i + 2)
        };

        DrawText(sizeText.c_str(), textPos.x, textPos.y, fontSize, textColor);
    }

    // Draw instructions
    const char* instructions = "Use UP/DOWN arrows to select, ENTER to confirm";
    int instructionSize = 20;
    Vector2 instructionsPos = {
        (GetScreenWidth() - MeasureText(instructions, instructionSize)) / 2.0f,
        GetScreenHeight() * 3.0f / 4.0f
    };
    DrawText(instructions, instructionsPos.x, instructionsPos.y, instructionSize, DARKGRAY);
}

void Game::draw() const {
    switch (state) {
        case GameState::MAP_SELECTION:
            drawMapSelection();
        break;

        case GameState::PLAYING:
            currentMap->draw();

        // Draw all placed towers
        for (const auto& tower : towerManager->getTowers()) {
            tower->draw();
        }

        drawTowerShots();
        drawTowerMenu();

        // Draw selection instruction if tower is selected
        if (!selectedTowerType.empty()) {
            DrawText("Click to place tower (Right click to cancel)",
                    GetScreenWidth() / 2 - 200, 40, 20, BLACK);
        }
        break;
    }
}