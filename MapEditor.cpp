/**
 * @file MapEditor.cpp
 * @brief Implementation of the MapEditor class for creating custom maps
 * @details This file provides the implementation for the interactive map editor,
 *          allowing users to create and modify maps for the tower defense game.
 */

#include "MapEditor.h"

/**
 * @brief Constructor for the MapEditor class
 * @param width Width of the map to be created
 * @param height Height of the map to be created
 * @details Initializes a new map editor with the specified dimensions, setting the default
 *          tool to PATH and initializing the editing state.
 */
MapEditor::MapEditor(int width, int height) {
    currentMap = new Map(width, height);
    currentTool = CellType::PATH;
    isDragging = false;
}

/**
 * @brief Destructor for the MapEditor class
 * @details Cleans up allocated memory for the map.
 */
MapEditor::~MapEditor() {
    delete currentMap;
    currentMap = nullptr;
}

/**
 * @brief Updates the map editor state based on user input
 * @details Handles mouse and keyboard input for map editing, including:
 *          - Tool selection (number keys 1-4)
 *          - Cell placement with dragging support
 *          - Coordinate conversion from screen to grid
 */
void MapEditor::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isDragging = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isDragging = false;
    }
    
    // Tool selection
    if (IsKeyPressed(KEY_ONE)) currentTool = CellType::PATH;
    if (IsKeyPressed(KEY_TWO)) currentTool = CellType::SCENERY;
    if (IsKeyPressed(KEY_THREE)) currentTool = CellType::ENTRY;
    if (IsKeyPressed(KEY_FOUR)) currentTool = CellType::EXIT;
    
    // Get mouse position and convert to grid coordinates
    Vector2 mousePos = GetMousePosition();
    int cellSize = std::min(GetScreenWidth() / (currentMap->getWidth() + 2),
                          GetScreenHeight() / (currentMap->getHeight() + 2));
    int offsetX = (GetScreenWidth() - (currentMap->getWidth() * cellSize)) / 2;
    int offsetY = (GetScreenHeight() - (currentMap->getHeight() * cellSize)) / 2;
    int gridX = (mousePos.x - offsetX) / cellSize;
    int gridY = (mousePos.y - offsetY) / cellSize;
    
    // Ensure grid coordinates are within bounds
    if (gridX < 0) gridX = 0;
    if (gridX >= currentMap->getWidth()) gridX = currentMap->getWidth() - 1;
    if (gridY < 0) gridY = 0;
    if (gridY >= currentMap->getHeight()) gridY = currentMap->getHeight() - 1;
    
    // Place cells while dragging
    if (isDragging || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (gridX >= 0 && gridX < currentMap->getWidth() &&
            gridY >= 0 && gridY < currentMap->getHeight()) {
            currentMap->setCellType(gridX, gridY, currentTool);
            }
    }
}

/**
 * @brief Renders the map editor interface
 * @details Draws the current map, tool selection UI, and validation status.
 *          The validation status indicates whether the map is valid for gameplay
 *          (has entry and exit points with a valid path between them).
 */
void MapEditor::draw() {
    // Draw the map
    currentMap->draw();
    
    // Draw UI elements
    DrawText("1: Path", 10, GetScreenHeight() - 100, 20, BLACK);
    DrawText("2: Scenery", 10, GetScreenHeight() - 75, 20, BLACK);
    DrawText("3: Entry", 10, GetScreenHeight() - 50, 20, BLACK);
    DrawText("4: Exit", 10, GetScreenHeight() - 25, 20, BLACK);
    
    // Draw validation status
    const char* status = currentMap->validateMap() ? "Map is valid" : "Map is invalid";
    DrawText(status, GetScreenWidth() - 200, GetScreenHeight() - 30, 20,
             currentMap->validateMap() ? GREEN : RED);
}
