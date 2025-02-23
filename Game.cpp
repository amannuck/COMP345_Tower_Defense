// Game.cpp
#include "Game.h"

Game::Game() : state(GameState::MAP_SELECTION), currentMap(nullptr), selectedSize(0) {
    mapSizes = {
        {12, 12},
        {14, 14},
        {16, 16},
    };
}

Game::~Game() {
    if (currentMap) delete currentMap;
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

        case GameState::PLAYING:
            // Handle tower placement
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                int cellSize = std::min(GetScreenWidth() / (currentMap->getWidth() + 2),
                                      GetScreenHeight() / (currentMap->getHeight() + 2));
                int offsetX = (GetScreenWidth() - (currentMap->getWidth() * cellSize)) / 2;
                int offsetY = (GetScreenHeight() - (currentMap->getHeight() * cellSize)) / 2;

                int gridX = (mousePos.x - offsetX) / cellSize;
                int gridY = (mousePos.y - offsetY) / cellSize;

                currentMap->placeTower(gridX, gridY);
            }
            break;
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
            // Draw game instructions
            DrawText("Left click to place towers", 10, 10, 20, BLACK);
            break;
    }
}