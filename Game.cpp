// Game.cpp
#include "Game.h"
#include "Tower.h"
#include <iostream>
#include <memory>
#include <ostream>

Game::Game() : state(GameState::MAP_SELECTION), currentMap(nullptr), selectedSize(0), isEditingWidth(true), currentWave(0), lives(10) {
    widthInput = "";
    heightInput = "";
    mapSizes = {
        {12, 12},
        {14, 14},
        {16, 16},
    };
    towerManager = std::make_unique<TowerManager>(500);
    critterWave = nullptr;
    mapEditor = nullptr;
}

Game::~Game() {
    delete currentMap;
    currentMap = nullptr;
    mapEditor = nullptr;  // Clean up mapEditor
}

void Game::showWaveNotification() {
    waveNotification.waveNumber = currentWave + 1; // Show next wave number
    waveNotification.timer = 2.0f; // Show for 2 seconds
    waveNotification.fadeDuration = 1.5f; // Start fading after 0.5 seconds
}

void Game::updateWaveNotification() {
    if (waveNotification.timer > 0) {
        waveNotification.timer -= GetFrameTime();
    }
}

void Game::drawWaveNotification() const {
    if (waveNotification.timer <= 0) return;

    // Calculate alpha (opacity) based on timer
    float alpha = 1.0f;
    if (waveNotification.timer < waveNotification.fadeDuration) {
        alpha = waveNotification.timer / waveNotification.fadeDuration;
    }

    std::string waveText = "Wave " + std::to_string(waveNotification.waveNumber);
    int fontSize = 80;
    Color textColor = { 255, 215, 0, static_cast<unsigned char>(255 * alpha) }; // Gold color with fade

    // Center the text
    Vector2 textPos = {
        (GetScreenWidth() - MeasureText(waveText.c_str(), fontSize)) / 2.0f,
        (GetScreenHeight() - fontSize) / 2.0f
    };

    // Draw text with shadow for better visibility
    DrawText(waveText.c_str(), textPos.x + 2, textPos.y + 2, fontSize, { 0, 0, 0, static_cast<unsigned char>(128 * alpha) });
    DrawText(waveText.c_str(), textPos.x, textPos.y, fontSize, textColor);
}


void Game::drawTowerMenu() const {
    // Draw bank balance at top left
    std::string bankText = "Bank: $" + std::to_string(towerManager->getCurrency());
    DrawText(bankText.c_str(), 10, 10, 30, RED);
    
    // Draw lives at top right
    std::string livesText = "Lives: " + std::to_string(lives);
    int textWidth = MeasureText(livesText.c_str(), 30);
    DrawText(livesText.c_str(), GetScreenWidth() - textWidth - sideMenuWidth - 10, 10, 30, RED);

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
        {"Slow", "Slow Tower", "DMG: 5\nSlow: 30%", 125, YELLOW},
        {"Sniper", "Sniper Tower", "DMG: 30\nRange: 300", 500, PURPLE}
    };

    for (int i = 0; i < 4; i++) {
        Rectangle button = {
            static_cast<float>(startX + i * (buttonWidth + padding)),
            static_cast<float>(startY),
            static_cast<float>(buttonWidth),
            static_cast<float>(buttonHeight)
        };

        // Button background
        Color buttonColor = options[i].color;
        if (selectedTowerType == options[i].type) {
            buttonColor.a = 285;  // Selected tower
        } else if (!towerManager->canAffordTower(options[i].type)) {
            buttonColor.a = 50;  // Can't afford
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
            selectedTower = dynamic_cast<Tower*>(tower.get());
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

    for (int i = 0; i < 4; i++) {
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
                case 3: towerType = "Sniper"; break;
            }

            if (towerManager->canAffordTower(towerType)) {
                selectedTowerType = (selectedTowerType == towerType) ? "" : towerType;
                selectedTower = nullptr; // Deselect tower when selecting tower type
            }
            break;
        }
    }
}


void Game::handleMapSelection() {
    if (state == GameState::MAP_SELECTION) {
        // Handle keyboard input for width and height
        int key = GetCharPressed(); // Get the character pressed by the user
        while (key > 0) {
            // Only allow numeric input (0-9)
            if (key >= '0' && key <= '9') {
                if (isEditingWidth) {
                    widthInput += static_cast<char>(key); // Append to width input
                } else {
                    heightInput += static_cast<char>(key); // Append to height input
                }
            }
            key = GetCharPressed(); // Get the next character
        }

        // Handle backspace to delete the last character
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (isEditingWidth && !widthInput.empty()) {
                widthInput.pop_back(); // Remove the last character from width input
            } else if (!isEditingWidth && !heightInput.empty()) {
                heightInput.pop_back(); // Remove the last character from height input
            }
        }

        // Handle TAB to switch between width and height input
        if (IsKeyPressed(KEY_TAB)) {
            isEditingWidth = !isEditingWidth; // Toggle between width and height input
        }

        // Handle ENTER to confirm dimensions
        if (IsKeyPressed(KEY_ENTER)) {
            // Convert width and height inputs to integers
            int width = widthInput.empty() ? 0 : std::stoi(widthInput);
            int height = heightInput.empty() ? 0 : std::stoi(heightInput);

            // Validate dimensions (ensure they are greater than 0)
            if (width > 0 && height > 0) {
                currentMap = new Map(width, height);
                mapEditor = std::make_unique<MapEditor>(width, height);
                state = GameState::MAP_EDITING;

                // Reset input fields for next use
                widthInput.clear();
                heightInput.clear();
            } else {
                std::cout << "Invalid dimensions! Width and height must be greater than 0." << std::endl;
            }
        }
    } else if (state == GameState::MAP_EDITING) {
        // Update the map editor
        mapEditor->update();

        // Check if the user has finished editing the map
        if (IsKeyPressed(KEY_ENTER)) {
            if (mapEditor->getMap()->validateMap()) {
                currentMap = new Map(*mapEditor->getMap());
                currentMap->calculatePath();
                state = GameState::PLAYING;
                mapEditor = nullptr;
            } else {
                std::cout << "Map is invalid! Please fix the errors." << std::endl;
            }
        }
    }
}

void Game::update() {
    updateWaveNotification();
    switch (state) {
        case GameState::MAP_SELECTION:
        case GameState::MAP_EDITING:
            handleMapSelection();
            break;

        case GameState::PLAYING: {
            UpdateTowerShots(GetFrameTime());

            Vector2 mousePos = GetMousePosition();
            bool processed = false;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (mousePos.y >= GetScreenHeight() - towerMenuHeight) {
                    handleTowerMenuClick(mousePos);
                    processed = true;
                } else if (mousePos.x >= GetScreenWidth() - sideMenuWidth) {
                    handleTowerInfoMenuClick(mousePos);
                    processed = true;
                } else if (!selectedTowerType.empty()) {
                    // Calculate available game area (excluding tower menu and always accounting for side menu)
                    int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
                    int gameAreaWidth = GetScreenWidth() - sideMenuWidth;

                    int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
                                          gameAreaHeight / (currentMap->getHeight() + 2));
                    int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
                    int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

                    int gridX = (mousePos.x - offsetX) / cellSize;
                    int gridY = (mousePos.y - offsetY) / cellSize;

                    if (gridX >= 0 && gridX < currentMap->getWidth() &&
                        gridY >= 0 && gridY < currentMap->getHeight() &&
                        currentMap->getCellType(gridX, gridY) == CellType::SCENERY) {
                        Vector2 towerPos = {
                            offsetX + gridX * cellSize + cellSize / 2.0f,
                            offsetY + gridY * cellSize + cellSize / 2.0f
                        };

                        if (towerManager->buyTower(selectedTowerType, towerPos)) {
                            currentMap->placeTower(gridX, gridY);
                            selectedTowerType = "";
                        }
                    }
                    processed = true;
                } else if (!processed) {
                    // If no tower type selected, check if we're selecting a tower
                    handleTowerSelection(mousePos);
                }
                handleSideMenuButtonClick(mousePos);
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                selectedTowerType = "";
                selectedTower = nullptr;  // Right-click also deselects tower
            }

            // Update critters only if the wave exists and has a valid path
            if (critterWave && !critterWave->getCritters().empty()) {
                critterWave->update(GetFrameTime());
            }

            updateTowers();
            break;
        }
        
        case GameState::GAME_OVER:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)) {
                // Reset game state and start new game
                state = GameState::PLAYING;
                resetLives();
                
                // Reset wave counter and possibly reset player's currency
                currentWave = 0;
                towerManager = std::make_unique<TowerManager>(500);
                
                // Clear any existing critters or towers
                critterWave = nullptr;
                selectedTower = nullptr;
                selectedTowerType = "";
            }
            break;
    }
    for (auto it = rewardNotifications.begin(); it != rewardNotifications.end();) {
        it->timer -= GetFrameTime();
        if (it->timer <= 0) {
            it = rewardNotifications.erase(it);
        } else {
            ++it;
        }
    }
}

bool Game::hasActiveCritters() const {
    if (!critterWave) return false;

    for (const auto& critter : critterWave->getCritters()) {
        // Check if critter is active AND not dead AND hasn't reached end
        if (critter.isActive() && !critter.isDead() && !critter.reachedEnd()) {
            return true;
        }
    }
    return false;
}

void Game::updateTowers() {
    for (const auto& tower : towerManager->getTowers()) {
        if (critterWave && !critterWave->getCritters().empty()) {
            tower->attackCritters(critterWave->getCritters());
        }
    }

    // Remove dead critters
    if (critterWave) {
        critterWave->removeDeadCritters();
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
    const char* title = "Enter Map Dimensions";
    int fontSize = 30;
    int spacing = 50;

    // Draw title
    Vector2 titlePos = {
        (GetScreenWidth() - MeasureText(title, fontSize)) / 2.0f,
        GetScreenHeight() / 4.0f
    };
    DrawText(title, titlePos.x, titlePos.y, fontSize, BLACK);

    // Draw width input field
    std::string widthPrompt = "Width: " + widthInput;
    Vector2 widthPos = {
        (GetScreenWidth() - MeasureText(widthPrompt.c_str(), fontSize)) / 2.0f,
        titlePos.y + spacing
    };
    DrawText(widthPrompt.c_str(), widthPos.x, widthPos.y, fontSize, isEditingWidth ? RED : BLACK);

    // Draw height input field
    std::string heightPrompt = "Height: " + heightInput;
    Vector2 heightPos = {
        (GetScreenWidth() - MeasureText(heightPrompt.c_str(), fontSize)) / 2.0f,
        widthPos.y + spacing
    };
    DrawText(heightPrompt.c_str(), heightPos.x, heightPos.y, fontSize, !isEditingWidth ? RED : BLACK);

    // Draw instructions
    const char* instructions = "Type width/height, press TAB to switch, ENTER to confirm";
    int instructionSize = 20;
    Vector2 instructionsPos = {
        (GetScreenWidth() - MeasureText(instructions, instructionSize)) / 2.0f,
        GetScreenHeight() * 3.0f / 4.0f
    };
    DrawText(instructions, instructionsPos.x, instructionsPos.y, instructionSize, DARKGRAY);
}

void Game::drawGameOver() const {
    // Draw a semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 200});
    
    // Draw game over text
    const char* gameOverText = "GAME OVER";
    int fontSize = 60;
    Vector2 textPos = {
        (GetScreenWidth() - MeasureText(gameOverText, fontSize)) / 2.0f,
        GetScreenHeight() / 3.0f
    };
    DrawText(gameOverText, textPos.x, textPos.y, fontSize, RED);
    
    // Draw wave reached text
    std::string waveText = "You reached wave: " + std::to_string(currentWave);
    int waveTextSize = 30;
    Vector2 waveTextPos = {
        (GetScreenWidth() - MeasureText(waveText.c_str(), waveTextSize)) / 2.0f,
        textPos.y + fontSize + 20
    };
    DrawText(waveText.c_str(), waveTextPos.x, waveTextPos.y, waveTextSize, WHITE);
    
    // Draw restart instructions
    const char* instructions = "Click or press ENTER to start a new game";
    int instructionSize = 20;
    Vector2 instructionsPos = {
        (GetScreenWidth() - MeasureText(instructions, instructionSize)) / 2.0f,
        GetScreenHeight() * 2.0f / 3.0f
    };
    DrawText(instructions, instructionsPos.x, instructionsPos.y, instructionSize, WHITE);
}

void Game::draw() const {
    switch (state) {
        case GameState::MAP_SELECTION:
            drawMapSelection();
            break;

        case GameState::MAP_EDITING:
            mapEditor->draw();  // Draw the map editor
            break;

        case GameState::PLAYING: {
            int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
            int gameAreaWidth = GetScreenWidth() - sideMenuWidth; // Always subtract side menu width

            int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
                                gameAreaHeight / (currentMap->getHeight() + 2));
            int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
            int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

            currentMap->draw(offsetX, offsetY, cellSize);

            // Draw all placed towers (positions will be consistent now)
            for (const auto& tower : towerManager->getTowers()) {
                // Add a highlight effect for the selected tower
                if (selectedTower == tower.get()) {
                    DrawCircleLines(tower->getPosition().x, tower->getPosition().y,
                                   20, WHITE);  // Highlight ring
                }
                tower->draw();
            }
            DrawTowerShots();
            // Draw critters
            if (critterWave && !critterWave->getCritters().empty()) {
                critterWave->draw();
            }

            // Always draw the side menu background
            drawSideMenu();
            drawSideMenuButton();

            // Only draw tower info if a tower is selected
            if (selectedTower) {
                drawTowerInfoInSideMenu();
            } else {
                drawSideMenuDefault(); // Draw default content when no tower is selected
            }
            drawTowerMenu();

            // Draw selection instruction if tower type is selected
            if (!selectedTowerType.empty()) {
                DrawText("Click to place tower (Right click to cancel)",
                        (GetScreenWidth() - sideMenuWidth) / 2 - 100, 20, 20, BLACK);
            }
            drawWaveNotification();
            break;
        }
            
        case GameState::GAME_OVER:
            drawGameOver();
            break;
    }
    for (const auto& notification : rewardNotifications) {
        float alpha = notification.timer > 1.0f ? 1.0f : notification.timer;

        // Use different colors for rewards and penalties
        Color textColor;
        std::string notificationText;

        if (notification.amount >= 0) {
            textColor = {255, 215, 0, static_cast<unsigned char>(255 * alpha)}; // Gold for rewards
            notificationText = "+" + std::to_string(notification.amount);
        } else {
            textColor = {255, 0, 0, static_cast<unsigned char>(255 * alpha)}; // Red for penalties
            notificationText = std::to_string(notification.amount); // Negative sign is already included
        }

        // Move the text upward as time passes
        float yOffset = (1.5f - notification.timer) * 30.0f;
        Vector2 textPos = {
            notification.position.x - MeasureText(notificationText.c_str(), 20) / 2.0f,
            notification.position.y - 30.0f - yOffset
        };

        DrawText(notificationText.c_str(), textPos.x, textPos.y, 20, textColor);
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

void Game::handleSideMenuButtonClick(Vector2 mousePos) {
    Rectangle buttonRect = {
        static_cast<float>(GetScreenWidth() - sideMenuWidth + 10),
        static_cast<float>(GetScreenHeight() - towerMenuHeight - 60),
        static_cast<float>(sideMenuWidth - 20),
        50
    };

    if (CheckCollisionPointRec(mousePos, buttonRect)) {
        // Only start next wave if no active critters
        if (!hasActiveCritters()) {
            startNextWave();
        }
        else {
            // Optionally: Show a message to the player
            std::cout << "Cannot start next wave while critters are still active!" << std::endl;
        }
    }
}

void Game::drawSideMenuButton() const {
    // Define the button rectangle
    Rectangle buttonRect = {
        static_cast<float>(GetScreenWidth() - sideMenuWidth + 10),
        static_cast<float>(GetScreenHeight() - towerMenuHeight - 60),
        static_cast<float>(sideMenuWidth - 20),
        50
    };

    // Check if the mouse is hovering over the button and if wave can be started
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), buttonRect);
    bool canStartWave = !hasActiveCritters();

    // Draw the button background
    Color buttonColor;
    if (canStartWave) {
        buttonColor = isHovered ? SKYBLUE : BLUE;
    } else {
        buttonColor = GRAY; // Disabled color
    }

    DrawRectangleRec(buttonRect, buttonColor);
    DrawRectangleLinesEx(buttonRect, 2, isHovered && canStartWave ? WHITE : BLACK);

    // Draw the button text
    const char* buttonText = "Next Wave";
    int fontSize = 20;
    float textWidth = MeasureText(buttonText, fontSize);

    // Use different text color based on button state
    Color textColor = canStartWave ? WHITE : DARKGRAY;
    DrawText(buttonText,
             buttonRect.x + (buttonRect.width - textWidth) / 2,
             buttonRect.y + (buttonRect.height - fontSize) / 2,
             fontSize, textColor);

    // Draw a tooltip if hovering over a disabled button
    if (isHovered && !canStartWave) {
        const char* tooltip = "Clear all critters first!";
        DrawText(tooltip, buttonRect.x, buttonRect.y - 25, 15, RED);
    }
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
    
    // Display current wave number
    std::string waveText = "Current Wave: " + std::to_string(currentWave);
    DrawText(waveText.c_str(), menuRect.x + 10, 210, 16, BLACK);
}

void Game::onCritterReachedEnd(const Critter& critter) {
    // Deduct a life
    lives--;
    
    // Check if game over
    if (lives <= 0) {
        state = GameState::GAME_OVER;
    }

    // Deduct currency based on critter's strength
    int penalty = critter.getStrength() * 10; // Multiply by 10 to make the penalty more significant
    int result = towerManager->getCurrency() - penalty;
    if (result < 0) {
        towerManager->addCurrency(0);
    } else {
        towerManager->addCurrency(-penalty);
    }
    std::cout << "Critter reached the end. Player loses " << penalty << " gold and 1 life! Lives remaining: " << lives << std::endl;

    // Add visual notification at the critter's position
    addRewardNotification(critter.getPosition(), -penalty);
}

void Game::onCritterDefeated(const Critter& critter) {
    // Reward the player when a critter is defeated
    int reward = critter.getReward();
    towerManager->addCurrency(reward);
    std::cout << "Critter defeated! Player receives " << reward << " gold!" << std::endl;

    // Add visual notification at the critter's position
    addRewardNotification(critter.getPosition(), reward);
}

void Game::addRewardNotification(const Vector2& position, int amount) {
    RewardNotification notification;
    notification.position = position;
    notification.amount = amount;
    notification.timer = 1.5f; // Display for 1.5 seconds
    rewardNotifications.push_back(notification);
}

void Game::startNextWave() {
    if (hasActiveCritters()) {
    std::cout << "Cannot start wave - active critters remain" << std::endl;
    return;
    }

    showWaveNotification();

    // Clear the previous wave if it exists
    if (critterWave) {
        critterWave.reset(); // This will delete the current wave
    }

    if (currentMap && !currentMap->getPath().empty()) {
        // Increment the current wave number
        currentWave++;

        // Calculate cell size and offsets
        int gameAreaHeight = GetScreenHeight() - towerMenuHeight;
        int gameAreaWidth = GetScreenWidth() - sideMenuWidth;
        int cellSize = std::min(gameAreaWidth / (currentMap->getWidth() + 2),
                            gameAreaHeight / (currentMap->getHeight() + 2));
        int offsetX = (gameAreaWidth - (currentMap->getWidth() * cellSize)) / 2;
        int offsetY = (gameAreaHeight - (currentMap->getHeight() * cellSize)) / 2;

        // Convert the path from grid coordinates to screen coordinates
        std::vector<Vector2> screenPath;
        for (const auto& point : currentMap->getPath()) {
            Vector2 screenPoint = {
                offsetX + point.x * cellSize + cellSize / 2.0f,
                offsetY + point.y * cellSize + cellSize / 2.0f
            };
            screenPath.push_back(screenPoint);
        }

        // Create the appropriate factory based on wave level
        auto factory = CritterFactoryCreator::createFactory(currentWave);

        // Determine number of critters based on wave level
        int critterCount = 5 + (currentWave * 2); // Example scaling formula

        // Create the wave of critters using the factory
        std::vector<Critter> critters = factory->createWave(screenPath, critterCount);

        // Initialize the critter wave with the created critters
        critterWave = std::make_unique<CritterWave>(currentWave, screenPath, cellSize, offsetX, offsetY);

        // Add the created critters to the wave
        for (auto& critter : critterWave->getCritters()) {

            // Register the game as an observer for each critter
            critter.addObserver(this);
        }
    } else {
        std::cerr << "ERROR: No map or path available to spawn critters." << std::endl;
    }
}
void Game::resetLives() {
    lives = 10;
    std::cout << "Lives reset to " << lives << std::endl;
}