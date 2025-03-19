// Map.cpp
#include "Map.h"

#include <iostream>
#include <ostream>

Map::Map(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<CellType>(width, CellType::SCENERY));  // Initialize all cells as SCENERY
    entryPoint = {-1, -1};
    exitPoint = {-1, -1};

    // Calculate cell size based on screen size and map dimensions
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    CELL_SIZE = std::min(screenWidth / (width + 2), screenHeight / (height + 2));
}

void Map::calculatePath() {
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<Vector2> queue;
    std::vector<std::vector<Vector2>> parent(height, std::vector<Vector2>(width, {-1, -1}));

    queue.push(entryPoint);
    visited[entryPoint.y][entryPoint.x] = true;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    bool pathFound = false;

    while (!queue.empty()) {
        Vector2 current = queue.front();
        queue.pop();

        // If we reach the exit point, the path is found
        if (current.x == exitPoint.x && current.y == exitPoint.y) {
            pathFound = true;
            break;
        }

        // Check all four directions
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValidCoordinate(newX, newY) && !visited[newY][newX] &&
                (grid[newY][newX] == CellType::PATH || grid[newY][newX] == CellType::EXIT)) {
                queue.push({static_cast<float>(newX), static_cast<float>(newY)});
                visited[newY][newX] = true;
                parent[newY][newX] = current;
            }
        }
    }

    if (pathFound) {
        // Reconstruct the path from exit to entry
        path.clear();
        Vector2 current = exitPoint;
        while (current.x != entryPoint.x || current.y != entryPoint.y) {
            path.push_back(current);
            current = parent[current.y][current.x];
        }
        path.push_back(entryPoint);
        std::reverse(path.begin(), path.end());
    } else {
        path.clear();
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

        // If we reach the exit point, the path is valid
        if (current.x == exitPoint.x && current.y == exitPoint.y) {
            return true;
        }

        // Check all four directions
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

void Map::draw(int offsetX, int offsetY, int cellSize) const {
    // If parameters aren't provided, calculate default values
    if (cellSize == 0) {
        cellSize = std::min(GetScreenWidth() / (width + 2), GetScreenHeight() / (height + 2));
        offsetX = (GetScreenWidth() - (width * cellSize)) / 2;
        offsetY = (GetScreenHeight() - (height * cellSize)) / 2;
    }

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
                case CellType::TOWER: break;
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

        if (type == CellType::ENTRY) {
            entryPoint = {static_cast<float>(x), static_cast<float>(y)};
            std::cout << "Entry Point Set: (" << entryPoint.x << ", " << entryPoint.y << ")\n";
        } else if (type == CellType::EXIT) {
            exitPoint = {static_cast<float>(x), static_cast<float>(y)};
            std::cout << "Exit Point Set: (" << exitPoint.x << ", " << exitPoint.y << ")\n";
        }
        notifyObservers();
    }
}

bool Map::validateMap() const {
    // Check if entry and exit points are set
    if (entryPoint.x == -1 || exitPoint.x == -1) {
        std::cerr << "Entry or exit point not set!" << std::endl;
        return false;
    }

    // Check if there's a valid path from entry to exit
    if (!hasValidPath()) {
        std::cerr << "No valid path from entry to exit!" << std::endl;
        return false;
    }

    return true;
}

void Map::setPath(const std::vector<Vector2>& path) {
    this->path = path;
}