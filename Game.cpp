// Game.cpp
#include "Game.h"
#include <memory>
#include <iostream>
#include <unistd.h>

Game::Game() : state(GameState::MAP_SELECTION), currentMap(nullptr), selectedSize(0), waveNumber(0), playerCurrency(1000), lives(10) {
    mapSizes = {
            {12, 12},
            {14, 14},
            {16, 16},
    };
    towerManager = std::make_unique<TowerManager>(1000);
}

Game::~Game() {
    delete currentMap;
    currentMap = nullptr;
}

void Game::drawTowerMenu() const {
    // Draw bank balance at top
    std::string bankText = "Bank: $" + std::to_string(towerManager->getCurrency());
    DrawText(bankText.c_str(), 10, 10, 30, RED);

    // Tower menu background (at the bottom of the screen)
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

void Game::drawTowerInfoInSideMenu() const {
    if (!selectedTower) return;

    // Side menu area
    Rectangle menuRect = {
            static_cast<float>(GetScreenWidth() - sideMenuWidth),
            0,
            static_cast<float>(sideMenuWidth),
            static_cast<float>(GetScreenHeight() - towerMenuHeight)
    };

    // Tower info
    int yPos = 70; // Start below the "Tower Info" title
    int padding = 10;
    int fontSize = 20;
    int smallFontSize = 16;


    // Title
    std::string titleText = selectedTower->getName();
    DrawText(titleText.c_str(), menuRect.x + padding, yPos, fontSize, BLACK);
    yPos += fontSize + padding;

    // Level
    std::string levelText = "Level: " + std::to_string(selectedTower->getLevel());
    DrawText(levelText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
    yPos += smallFontSize + padding;

    // Stats
    std::string powerText = "Power: " + std::to_string(static_cast<int>(selectedTower->getPower()));
    DrawText(powerText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
    yPos += smallFontSize + padding;

    std::string rangeText = "Range: " + std::to_string(static_cast<int>(selectedTower->getRange()));
    DrawText(rangeText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
    yPos += smallFontSize + padding;

    std::string rateText = "Fire Rate: " + std::to_string(selectedTower->getFireRate());

    DrawText(rateText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
    yPos += smallFontSize + padding * 2;

    // Special stats for specific tower types
    if (dynamic_cast<AreaTower*>(selectedTower)) {
        AreaTower* areaTower = dynamic_cast<AreaTower*>(selectedTower);
        std::string areaText = "Area: " + std::to_string(static_cast<int>(areaTower->getAreaRadius()));
        DrawText(areaText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
        yPos += smallFontSize + padding;
    } else if (dynamic_cast<SlowTower*>(selectedTower)) {
        SlowTower* slowTower = dynamic_cast<SlowTower*>(selectedTower);
        std::string slowText = "Slow: " + std::to_string(static_cast<int>(slowTower->getSlowEffect() * 100)) + "%";
        DrawText(slowText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
        yPos += smallFontSize + padding;

        std::string durationText = "Duration: " + std::to_string(static_cast<int>(slowTower->getSlowDuration() * 10) / 10.0f) + "s";
        DrawText(durationText.c_str(), menuRect.x + padding, yPos, smallFontSize, BLACK);
        yPos += smallFontSize + padding;
    }

    yPos += padding * 2;

    // Upgrade button
    int buttonSpacing = 20;
    int buttonHeight = 50;
    Rectangle upgradeButton = {
            menuRect.x + padding,
            static_cast<float>(yPos),
            sideMenuWidth - padding * 2,
            50
    };

    bool upgradeHovered = CheckCollisionPointRec(GetMousePosition(), upgradeButton);
    bool canAfford = towerManager->canAffordUpgrade(selectedTower);
    Color buttonColor = canAfford ? GREEN : GRAY;
    Color outlineColor = upgradeHovered ? WHITE : BLACK;

    DrawRectangleRec(upgradeButton, buttonColor);
    DrawRectangleLinesEx(upgradeButton, upgradeHovered ? 3 : 2, outlineColor);

    std::string upgradeText = "Upgrade: $" + std::to_string(selectedTower->getUpgradeCost());
    float textWidth = MeasureText(upgradeText.c_str(), smallFontSize);
    DrawText(upgradeText.c_str(),
             upgradeButton.x + (upgradeButton.width - textWidth) / 2,
             upgradeButton.y + (upgradeButton.height - smallFontSize) / 2,
             smallFontSize, BLACK);

    yPos += buttonHeight + buttonSpacing;

    // Sell button
    Rectangle sellButton = {
            menuRect.x + padding,
            static_cast<float>(yPos),
            sideMenuWidth - padding * 2,
            50
    };

    bool sellHovered = CheckCollisionPointRec(GetMousePosition(), sellButton);
    outlineColor = sellHovered ? WHITE : BLACK;

    DrawRectangleRec(sellButton, RED);
    DrawRectangleLinesEx(sellButton, 2, BLACK);

    std::string sellText = "Sell: $" + std::to_string(selectedTower->getRefundValue());
    textWidth = MeasureText(sellText.c_str(), smallFontSize);
    DrawText(sellText.c_str(),
             sellButton.x + (sellButton.width - textWidth) / 2,
             sellButton.y + (sellButton.height - smallFontSize) / 2,
             smallFontSize, BLACK);

    // Show hover effects for buttons
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, upgradeButton)) {
        DrawRectangleLinesEx(upgradeButton, 2, WHITE);
    }
    if (CheckCollisionPointRec(mousePos, sellButton)) {
        DrawRectangleLinesEx(sellButton, 2, WHITE);
    }
}

void Game::handleTowerInfoMenuClick(Vector2 mousePos) {
    if (!selectedTower) return;

    // Check if click is in the side menu area
    Rectangle menuRect = {
            static_cast<float>(GetScreenWidth() - sideMenuWidth),
            0,
            static_cast<float>(sideMenuWidth),
            static_cast<float>(GetScreenHeight() - towerMenuHeight)
    };

    if (!CheckCollisionPointRec(mousePos, menuRect)) return;

    int padding = 10;
    int yPos = 70; // Match the starting position in drawTowerInfoInSideMenu
    int fontSize = 20;
    int smallFontSize = 16;

    // Skip past all the text elements
    yPos += fontSize + padding; // Title
    yPos += smallFontSize + padding; // Level
    yPos += smallFontSize + padding; // Power
    yPos += smallFontSize + padding; // Range
    yPos += smallFontSize + padding; // Fire Rate
    yPos += smallFontSize + padding * 2; // Extra spacing

    // Check for special tower types and skip their stats
    if (dynamic_cast<AreaTower*>(selectedTower)) {
        yPos += smallFontSize + padding;
    } else if (dynamic_cast<SlowTower*>(selectedTower)) {
        yPos += smallFontSize + padding;
        yPos += smallFontSize + padding;
    }

    yPos += padding * 2;

    // Upgrade button
    Rectangle upgradeButton = {
            menuRect.x + padding,
            static_cast<float>(yPos),
            sideMenuWidth - padding * 2,
            50
    };

    if (CheckCollisionPointRec(mousePos, upgradeButton)) {
        if (towerManager->canAffordUpgrade(selectedTower)) {
            towerManager->upgradeTower(selectedTower);
        }
        return;
    }

    yPos += 50 + padding;

    // Sell button
    Rectangle sellButton = {
            menuRect.x + padding,
            static_cast<float>(yPos),
            sideMenuWidth - padding * 2,
            50
    };


    if (CheckCollisionPointRec(mousePos, sellButton)) {
        // Get the tower's position
        Vector2 towerPos = selectedTower->getPosition();

        // Calculate grid coordinates from the tower's position
        int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
        int gameAreaWidth = GetScreenWidth() - sideMenuWidth;
        int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
                                gameAreaHeight / (currentMap->getHeight() + 2));
        int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
        int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

        // Convert tower position to grid coordinates
        int gridX = (towerPos.x - offsetX) / cellSize;
        int gridY = (towerPos.y - offsetY) / cellSize;

        // Reset the cell to scenery
        if (gridX >= 0 && gridX < currentMap->getWidth() &&
            gridY >= 0 && gridY < currentMap->getHeight()) {
            currentMap->setCellType(gridX, gridY, CellType::SCENERY);
        }

        if (selectedTower) {
            towerManager->sellTower(selectedTower);
            selectedTower = nullptr;
        }
        return;
    }
}


void Game::handleTowerSelection(Vector2 mousePos) {
    // Don't process tower selection if click is in the tower menu
    if (mousePos.y >= GetScreenHeight() - towerMenuHeight) return;

    // Don't process tower selection if click is in the side menu
    if (mousePos.x >= GetScreenWidth() - sideMenuWidth) return;

    // Calculate cell size based on available game area
    int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
    int gameAreaWidth = GetScreenWidth() - sideMenuWidth;  // Always account for side menu

    int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
                            gameAreaHeight / (currentMap->getHeight() + 2));
    int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
    int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

    // Check if we clicked on a tower
    selectedTower = nullptr;
    for (const auto& tower : towerManager->getTowers()) {
        Vector2 pos = tower->getPosition();
        float radius = 15.0f;  // Tower radius from draw method

        if (CheckCollisionPointCircle(mousePos, pos, radius)) {
            selectedTower = tower.get();
            break;
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
                selectedTower = nullptr; // Deselect tower when selecting tower type
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
    startNewWave();  // Start the first wave
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

void Game::update(float deltaTime) {
    switch (state) {
        case GameState::MAP_SELECTION:
            handleMapSelection();
            break;

        case GameState::PLAYING: {
            Vector2 mousePos = GetMousePosition();
            bool processed = false;

            // 🏹 Handle Mouse Input for Tower Selection/Placement
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (mousePos.y >= GetScreenHeight() - towerMenuHeight) {
                    handleTowerMenuClick(mousePos);
                    processed = true;
                } else if (mousePos.x >= GetScreenWidth() - sideMenuWidth) {
                    handleTowerInfoMenuClick(mousePos);
                    processed = true;
                } else if (!selectedTowerType.empty()) {
                    placeTower(mousePos);
                    processed = true;
                } else if (!processed){
                    handleTowerSelection(mousePos);
                }
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                selectedTowerType = "";
                selectedTower = nullptr;  // Right-click deselects tower
            }

            // 🚀 Update Critters (New)
            if (currentWave) {
                currentWave->update(deltaTime);

                std::vector<Critter>& critters = currentWave->getCritters();
                auto it = critters.begin();
                while (it != critters.end()) {
                    if (it->isDead()) {
                        std::cout << "Critter defeated! + " << it->getReward() << " coins.\n";
                        playerCurrency += it->getReward();
                        it = critters.erase(it);
                    } else if (it->reachedEnd()) {
                        std::cout << "Critter reached exit! -" << it->getStrength() << " lives.\n";
                        lives -= it->getStrength();
                        it = critters.erase(it);
                    } else {
                        ++it;
                    }
                }

                // Delay before starting new wave (if critters are still spawning, wait)
                if (currentWave->allCrittersDefeated() && !currentWave->isStillSpawning()) {
                    std::cout << "Wave " << waveNumber << " completed. Next wave in 3 seconds...\n";
                    sleep(3);
                    startNewWave();
                }
            }

            // ⚠️ Check for Game Over (New)
            checkGameOver();

            // 🔥 Update Towers
            updateTowers();
            break;
        }

        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) {
                restartGame();
            }
            break;
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

//void Game::draw() const {
//    switch (state) {
//        case GameState::MAP_SELECTION:
//            drawMapSelection();
//            break;
//
//        case GameState::PLAYING: {
//            // Always account for the side menu width in calculations
//            int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
//            int gameAreaWidth = GetScreenWidth() - sideMenuWidth; // Always subtract side menu width
//
//            int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
//                                    gameAreaHeight / (currentMap->getHeight() + 2));
//            int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
//            int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;
//
//            // 🗺️ Draw the Map
//            currentMap->draw(offsetX, offsetY, cellSize);
//
//            // 🏰 Draw Towers
//            for (const auto& tower : towerManager->getTowers()) {
//                if (selectedTower == tower.get()) {
//                    DrawCircleLines(tower->getPosition().x, tower->getPosition().y, 20, WHITE); // Highlight effect
//                }
//                tower->draw();
//            }
//
//            // 👾 Draw Critters
//            if (currentWave) {
//                currentWave->draw();
//            }
//
//            // 📜 Draw HUD (Stats UI)
//            DrawText(("Wave: " + std::to_string(waveNumber)).c_str(), 20, 20, 30, WHITE);
//            DrawText(("Coins: $" + std::to_string(playerCurrency)).c_str(), 20, 60, 30, YELLOW);
//            DrawText(("Lives: " + std::to_string(lives)).c_str(), 20, 100, 30, RED);
//
//            // 📋 Draw UI Elements
//            drawSideMenu();
//
//            if (selectedTower) {
//                drawTowerInfoInSideMenu();
//            } else {
//                drawSideMenuDefault();
//            }
//
//            drawTowerShots();
//            drawTowerMenu();
//
//            // 🏗️ Tower Placement Instructions
//            if (!selectedTowerType.empty()) {
//                DrawText("Click to place tower (Right click to cancel)",
//                         (GetScreenWidth() - sideMenuWidth) / 2 - 100, 20, 20, BLACK);
//            }
//            break;
//        }
//
//        case GameState::GAME_OVER:
//            // 🎮 Display Game Over Screen
//            DrawText("GAME OVER", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 - 50, 50, RED);
//            DrawText("Press ENTER to Restart", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 10, 20, WHITE);
//            break;
//    }
//}

void Game::draw() const {
    switch (state) {
        case GameState::MAP_SELECTION:
            drawMapSelection();
            break;

        case GameState::PLAYING: {
            // Always account for the side menu width in calculations
            int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
            int gameAreaWidth = GetScreenWidth() - sideMenuWidth; // Always subtract side menu width

            int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
                                    gameAreaHeight / (currentMap->getHeight() + 2));
            int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
            int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

            // 🗺️ Draw the Map
            currentMap->draw(offsetX, offsetY, cellSize);

            // 🏰 Draw Towers
            for (const auto& tower : towerManager->getTowers()) {
                if (selectedTower == tower.get()) {
                    DrawCircleLines(tower->getPosition().x, tower->getPosition().y, 20, WHITE); // Highlight effect
                }
                tower->draw();
            }

            // 👾 Draw Critters (Ensure they are being drawn!)
            if (currentWave) {
                std::cout << "✅ Calling currentWave->draw(). Critters in wave: "
                          << currentWave->getCritters().size() << std::endl;
                currentWave->draw();
            } else {
                std::cout << "❌ currentWave is nullptr!" << std::endl;
            }

            // 📜 Draw HUD (Stats UI)
            DrawText(("Wave: " + std::to_string(waveNumber)).c_str(), 20, 20, 30, WHITE);
            DrawText(("Coins: $" + std::to_string(playerCurrency)).c_str(), 20, 60, 30, YELLOW);
            DrawText(("Lives: " + std::to_string(lives)).c_str(), 20, 100, 30, RED);

            // 📋 Draw UI Elements
            drawSideMenu();

            if (selectedTower) {
                drawTowerInfoInSideMenu();
            } else {
                drawSideMenuDefault();
            }

            drawTowerShots();
            drawTowerMenu();

            // 🏗️ Tower Placement Instructions
            if (!selectedTowerType.empty()) {
                DrawText("Click to place tower (Right click to cancel)",
                         (GetScreenWidth() - sideMenuWidth) / 2 - 100, 20, 20, BLACK);
            }
            break;
        }

        case GameState::GAME_OVER:
            // 🎮 Display Game Over Screen
            DrawText("GAME OVER", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 - 50, 50, RED);
            DrawText("Press ENTER to Restart", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 10, 20, WHITE);
            break;
    }
}


void Game::drawSideMenu() const {
    // Side menu background
    Rectangle menuRect = {
            static_cast<float>(GetScreenWidth() - sideMenuWidth),
            0,
            static_cast<float>(sideMenuWidth),
            static_cast<float>(GetScreenHeight() - towerMenuHeight)
    };

    DrawRectangleRec(menuRect, LIGHTGRAY);
    DrawRectangleLinesEx(menuRect, 2, BLACK);

    // Draw a title for the side menu
    DrawText("Tower Info", menuRect.x + 10, 20, 24, BLACK);
}

void Game::drawSideMenuDefault() const {
    Rectangle menuRect = {
            static_cast<float>(GetScreenWidth() - sideMenuWidth),
            0,
            static_cast<float>(sideMenuWidth),
            static_cast<float>(GetScreenHeight() - towerMenuHeight)
    };

    // Display instructions text
    DrawText("Select a tower", menuRect.x + 10, 70, 20, DARKGRAY);
    DrawText("to view info", menuRect.x + 10, 100, 20, DARKGRAY);

    // Maybe add some general game stats
    DrawText("Game Stats:", menuRect.x + 10, 150, 18, BLACK);

    // Example stats (replace with actual game stats)
    std::string towersText = "Towers: " + std::to_string(towerManager->getTowers().size());
    DrawText(towersText.c_str(), menuRect.x + 10, 180, 16, BLACK);
}

//void Game::startNewWave() {
//    if (!currentMap) {
//        std::cerr << "ERROR: Cannot start a new wave, currentMap is null!" << std::endl;
//        return;
//    }
//
//    std::vector<Vector2> path = currentMap->getPath();  // ✅ Retrieve path from map
//    if (path.empty()) {
//        std::cerr << "ERROR: Cannot start a new wave, path is empty!" << std::endl;
//        return;
//    }
//
//    std::cout << "Starting new wave with " << path.size() << " path points." << std::endl;
//
//    currentWave = std::make_unique<CritterWave>(waveNumber, path);
//    waveNumber++;
//}
void Game::startNewWave() {
    if (!currentMap) {
        std::cerr << "ERROR: Cannot start a new wave, currentMap is null!" << std::endl;
        return;
    }

    std::vector<Vector2> path = currentMap->getPath();
    if (path.empty()) {
        std::cerr << "ERROR: Cannot start a new wave, path is empty!" << std::endl;
        return;
    }

    std::cout << "🌊 Starting Wave " << waveNumber + 1 << "..." << std::endl;
    currentWave = std::make_unique<CritterWave>(waveNumber, path);
    waveNumber++;
}






void Game::checkGameOver() {
    if (lives <= 0) {
        state = GameState::GAME_OVER;
    }
}

void Game::restartGame() {
    std::cout << "Restarting game...\n";
    state = GameState::MAP_SELECTION;
    waveNumber = 0;
    lives = 10;
    playerCurrency = 1000;
    towerManager = std::make_unique<TowerManager>(1000);
    currentWave.reset();
}

void Game::placeTower(Vector2 mousePos) {
    int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
    int gameAreaWidth = GetScreenWidth() - sideMenuWidth;
    int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2), gameAreaHeight / (currentMap->getHeight() + 2));
    int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
    int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

    int gridX = (mousePos.x - offsetX) / cellSize;
    int gridY = (mousePos.y - offsetY) / cellSize;

    if (gridX >= 0 && gridX < currentMap->getWidth() && gridY >= 0 && gridY < currentMap->getHeight() &&
        currentMap->getCellType(gridX, gridY) == CellType::SCENERY) {
        Vector2 towerPos = {offsetX + gridX * cellSize + cellSize / 2.0f, offsetY + gridY * cellSize + cellSize / 2.0f};
        if (towerManager->buyTower(selectedTowerType, towerPos)) {
            currentMap->placeTower(gridX, gridY);
            selectedTowerType = "";
        }
    }
}





