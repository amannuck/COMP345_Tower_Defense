#include "MapEditor.h"

MapEditor::MapEditor(int width, int height) {
    currentMap = new Map(width, height);
    currentTool = CellType::PATH;
    isDragging = false;
}

MapEditor::~MapEditor() {
    delete currentMap;
    currentMap = nullptr;
}

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

    // Place cells while dragging
    if (isDragging || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (gridX >= 0 && gridX < currentMap->getWidth() &&
            gridY >= 0 && gridY < currentMap->getHeight()) {
            currentMap->setCellType(gridX, gridY, currentTool);
        }
    }
}

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