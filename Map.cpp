// Map.cpp
#include "Map.h"
#include <iostream>

Map::Map(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<CellType>(width, CellType::SCENERY));
    entryPoint = {-1, -1};
    exitPoint = {-1, -1};

    // Calculate cell size based on screen size and map dimensions
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    CELL_SIZE = std::min(screenWidth / (width + 2), screenHeight / (height + 2));
}


//void Map::generateRandomMap() {
//    srand(time(0));
//
//    // Reset map to all scenery
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            grid[y][x] = CellType::SCENERY;
//        }
//    }
//
//    // Ensure the path starts at a valid position on the leftmost column
//    int entryY = rand() % height;  // Randomize initial Y position
//    while (entryY >= height - 2) entryY = rand() % height;  // Avoid bottom edge
//
//    grid[entryY][0] = CellType::ENTRY;
//    entryPoint = {0, static_cast<float>(entryY)}; // Set the entry point position
//
//    // Set exit point on rightmost column, ensuring it's a valid path location
//    int exitY = rand() % height;
//    while (exitY == entryY || exitY >= height - 2) exitY = rand() % height; // Avoid same row as entry
//
//    grid[exitY][width - 1] = CellType::EXIT;
//    exitPoint = {static_cast<float>(width - 1), static_cast<float>(exitY)}; // Set the exit point position
//
//    // Generate a random path from entry to exit
//    int currentX = 0;
//    int currentY = entryY;
//
//    while (currentX < width - 1 || currentY != exitY) {
//        grid[currentY][currentX] = CellType::PATH;
//
//        if (currentX == width - 1) {
//            // Move only vertically if at rightmost column
//            currentY += (exitY > currentY) ? 1 : -1;
//        } else if (currentY == exitY) {
//            // Move only horizontally if already at exit row
//            currentX++;
//        } else {
//            // Randomly choose to move horizontally or vertically
//            int direction = rand() % 2;
//            if (direction == 0) {
//                currentX++;  // Move right
//            } else {
//                currentY += (exitY > currentY) ? 1 : -1; // Move toward exit row
//            }
//        }
//    }
//
//    // Ensure last cell before exit is a path
//    grid[exitY][width - 2] = CellType::PATH;
//
//    // Mark final entry and exit points
//    grid[entryY][0] = CellType::ENTRY;
//    grid[exitY][width - 1] = CellType::EXIT;
//}
void Map::generateRandomMap() {
    srand(time(0));

    std::cout << "Generating Random Map..." << std::endl;

    // Reset map
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = CellType::SCENERY;
        }
    }

    // Ensure the path starts at a valid position
    int entryY = rand() % height;
    while (entryY >= height - 2) entryY = rand() % height;

    grid[entryY][0] = CellType::ENTRY;
    entryPoint = {0, static_cast<float>(entryY)};

    int exitY = rand() % height;
    while (exitY == entryY || exitY >= height - 2) exitY = rand() % height;

    grid[exitY][width - 1] = CellType::EXIT;
    exitPoint = {static_cast<float>(width - 1), static_cast<float>(exitY)};

    std::cout << "Entry: (" << entryPoint.x << ", " << entryPoint.y << ")" << std::endl;
    std::cout << "Exit: (" << exitPoint.x << ", " << exitPoint.y << ")" << std::endl;

    // Generate a random path
    int currentX = 0;
    int currentY = entryY;

    path.clear();
    path.push_back(entryPoint);

    while (currentX < width - 1 || currentY != exitY) {
        grid[currentY][currentX] = CellType::PATH;

        if (currentX == width - 1) {
            currentY += (exitY > currentY) ? 1 : -1;
        } else if (currentY == exitY) {
            currentX++;
        } else {
            int direction = rand() % 2;
            if (direction == 0) currentX++;
            else currentY += (exitY > currentY) ? 1 : -1;
        }

        path.push_back({static_cast<float>(currentX), static_cast<float>(currentY)});
    }

    // Ensure the last cell before exit is part of the path
    grid[exitY][width - 2] = CellType::PATH;

    // Final check for path validity
    if (path.empty()) {
        std::cerr << "❌ ERROR: Map path is empty! Critters cannot spawn." << std::endl;
    } else {
        std::cout << "✅ Path generated with " << path.size() << " waypoints." << std::endl;
    }
}


bool Map::hasValidPath() const {
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<Vector2> queue;
    queue.push(entryPoint);
    visited[entryPoint.y][entryPoint.x] = true;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    while (!queue.empty()) {
        Vector2 current = queue.front();
        queue.pop();

        if (current.x == exitPoint.x && current.y == exitPoint.y) {
            return true;
        }

        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValidCoordinate(newX, newY) && !visited[newY][newX] &&
                (grid[newY][newX] == CellType::PATH || grid[newY][newX] == CellType::EXIT)) {
                queue.push({static_cast<float>(newX), static_cast<float>(newY)});
                visited[newY][newX] = true;
            }
        }
    }
    return false;
}

CellType Map::getCellType(int x, int y) const {
    if (isValidCoordinate(x, y)) {
        return grid[y][x];
    }
    return CellType::SCENERY;
}

bool Map::placeTower(int x, int y) {
    if (!isValidCoordinate(x, y)) return false;
    if (grid[y][x] != CellType::SCENERY) return false;

    grid[y][x] = CellType::TOWER;
    return true;
}


void Map::draw(int offsetX, int offsetY, int cellSize) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Rectangle cellRect = {
                    static_cast<float>(offsetX + x * cellSize),
                    static_cast<float>(offsetY + y * cellSize),
                    static_cast<float>(cellSize),
                    static_cast<float>(cellSize)
            };

            Color cellColor;
            switch (grid[y][x]) {
                case CellType::SCENERY: cellColor = GREEN; break;
                case CellType::PATH: cellColor = BROWN; break;
                case CellType::ENTRY: cellColor = BLUE; break;  // ✅ ENTRY is BLUE
                case CellType::EXIT: cellColor = RED; break;    // ✅ EXIT is RED
                case CellType::TOWER: cellColor = GRAY; break;
                default: cellColor = LIGHTGRAY;
            }

            DrawRectangleRec(cellRect, cellColor);
            DrawRectangleLinesEx(cellRect, 1, BLACK);
        }
    }
}

//void Map::draw(int offsetX, int offsetY, int cellSize) const {
//    if (cellSize == 0) {
//        cellSize = std::min(GetScreenWidth() / (width + 2), GetScreenHeight() / (height + 2));
//        offsetX = (GetScreenWidth() - (width * cellSize)) / 2;
//        offsetY = (GetScreenHeight() - (height * cellSize)) / 2;
//    }
//
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            Rectangle cellRect = {
//                    static_cast<float>(offsetX + x * cellSize),
//                    static_cast<float>(offsetY + y * cellSize),
//                    static_cast<float>(cellSize),
//                    static_cast<float>(cellSize)
//            };
//
//            Color cellColor;
//            switch (grid[y][x]) {
//                case CellType::SCENERY: cellColor = GREEN; break;
//                case CellType::PATH: cellColor = BROWN; break;
//                case CellType::ENTRY: cellColor = BLUE; break;
//                case CellType::EXIT: cellColor = RED; break;
//                case CellType::TOWER: cellColor = GRAY; break;
//                default: cellColor = LIGHTGRAY;
//            }
//
//            DrawRectangleRec(cellRect, cellColor);
//            DrawRectangleLinesEx(cellRect, 1, BLACK);
//
//            // 🔹 Draw 'S' for Start, 'E' for Exit
//            if (grid[y][x] == CellType::ENTRY) {
//                DrawText("S", cellRect.x + cellSize / 4, cellRect.y + cellSize / 4, cellSize / 2, WHITE);
//            } else if (grid[y][x] == CellType::EXIT) {
//                DrawText("E", cellRect.x + cellSize / 4, cellRect.y + cellSize / 4, cellSize / 2, WHITE);
//            }
//        }
//    }
//}

// Replace your current Map::draw() function with this version
//void Map::draw(int offsetX, int offsetY, int cellSize) const {
//    // If parameters aren't provided, calculate default values
//    if (cellSize == 0) {
//        cellSize = std::min(GetScreenWidth() / (width + 2), GetScreenHeight() / (height + 2));
//        offsetX = (GetScreenWidth() - (width * cellSize)) / 2;
//        offsetY = (GetScreenHeight() - (height * cellSize)) / 2;
//    }
//
//    // Draw the map grid with the provided parameters
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            Rectangle cellRect = {
//                    static_cast<float>(offsetX + x * cellSize),
//                    static_cast<float>(offsetY + y * cellSize),
//                    static_cast<float>(cellSize),
//                    static_cast<float>(cellSize)
//            };
//
//            Color cellColor;
//            switch (grid[y][x]) {
//                case CellType::SCENERY: cellColor = GREEN; break;
//                case CellType::PATH: cellColor = BROWN; break;
//                case CellType::ENTRY: cellColor = BLUE; break;
//                case CellType::EXIT: cellColor = RED; break;
//                case CellType::TOWER: cellColor = GRAY; break;
//                default: cellColor = LIGHTGRAY;
//            }
//
//            DrawRectangleRec(cellRect, cellColor);
//            DrawRectangleLinesEx(cellRect, 1, BLACK);
//        }
//    }
//}
void Map::setCellType(int x, int y, CellType type) {
    if (isValidCoordinate(x, y)) {
        grid[y][x] = type;

        // Update entry/exit points if necessary
        if (type == CellType::ENTRY) {
            entryPoint = {static_cast<float>(x), static_cast<float>(y)};
        }
        else if (type == CellType::EXIT) {
            exitPoint = {static_cast<float>(x), static_cast<float>(y)};
        }
    }
}

bool Map::validateMap() const {
    // Check if entry and exit points are set
    if (entryPoint.x == -1 || exitPoint.x == -1) {
        return false;
    }

    // Check if there's a valid path
    return hasValidPath();
}