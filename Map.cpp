// Map.cpp
#include "Map.h"

Map::Map(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<CellType>(width, CellType::SCENERY));
    entryPoint = {-1, -1};
    exitPoint = {-1, -1};

    // Calculate cell size based on screen size and map dimensions
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    CELL_SIZE = std::min(screenWidth / (width + 2), screenHeight / (height + 2));
}

void Map::generateRandomMap() {
    srand(time(0));

    // Reset map to all scenery
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = CellType::SCENERY;
        }
    }

    // Set entry point on left side
    int entryY = rand() % height;
    entryPoint = {0, static_cast<float>(entryY)};
    grid[entryY][0] = CellType::ENTRY;

    // Set exit point on right side
    int exitY = rand() % height;
    exitPoint = {static_cast<float>(width - 1), static_cast<float>(exitY)};
    grid[exitY][width - 1] = CellType::EXIT;

    // Generate path from entry to exit using improved directional logic
    int currentX = 0;  // Start at entry point
    int currentY = entryY;

    while (currentX != width - 1 || currentY != exitY) {
        grid[currentY][currentX] = CellType::PATH;

        // Decide movement direction based on current position relative to exit
        if (currentX == width - 1) {
            // If at rightmost column, only move vertically towards exit
            currentY += (exitY > currentY) ? 1 : -1;
        } else if (currentY == exitY) {
            // If at same Y as exit, move horizontally
            currentX++;
        } else {
            // Randomly choose horizontal or vertical movement
            int direction = rand() % 2;

            if (direction == 0) {
                // Move horizontally
                currentX++;
            } else {
                // Move vertically towards exit
                currentY += (exitY > currentY) ? 1 : -1;
            }
        }
    }

    // Ensure the exit cell is marked as PATH before setting it as EXIT
    grid[exitY][width - 1] = CellType::PATH;

    // Reset entry and exit points with correct cell types
    grid[entryY][0] = CellType::ENTRY;
    grid[exitY][width - 1] = CellType::EXIT;

    notifyObservers();
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

    notifyObservers();
    return true;
}

// Replace your current Map::draw() function with this version
void Map::draw(int offsetX, int offsetY, int cellSize) const {
    // If parameters aren't provided, calculate default values
    if (cellSize == 0) {
        cellSize = std::min(GetScreenWidth() / (width + 2), GetScreenHeight() / (height + 2));
        offsetX = (GetScreenWidth() - (width * cellSize)) / 2;
        offsetY = (GetScreenHeight() - (height * cellSize)) / 2;
    }

    // Draw the map grid with the provided parameters
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
                case CellType::ENTRY: cellColor = BLUE; break;
                case CellType::EXIT: cellColor = RED; break;
                case CellType::TOWER: cellColor = GRAY; break;
                default: cellColor = LIGHTGRAY;
            }

            DrawRectangleRec(cellRect, cellColor);
            DrawRectangleLinesEx(cellRect, 1, BLACK);
        }
    }
}
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

        notifyObservers();
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