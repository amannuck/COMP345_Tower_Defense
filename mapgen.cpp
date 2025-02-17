/**
 * @file mapgen.cpp
 * @brief Implementation of the Map class for the Tower Defense game.
 */

#include "mapgen.h"
#include <cstdlib>  // rand()
#include <ctime>    // time()

/**
 * @brief Constructs a new Map object with given dimensions.
 * @param w Width of the map.
 * @param h Height of the map.
 */
Map::Map(int w, int h) {
    width = w;
    height = h;
    entrySet = false;
    exitSet = false;

    // Create a 2D grid filled with SCENERY
    grid.resize(height, vector<CellType>(width, SCENERY));
}

/**
 * @brief Sets a cell as PATH if coordinates are valid.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 */
 void Map::setPath(int x, int y) {
    if (isValidCoordinate(x, y)) {
        grid[y][x] = PATH;
    }
}

/**
 * @brief Sets the entry point if it's a valid PATH cell.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 */
 void Map::setEntry(int x, int y) {
    if (isValidCoordinate(x, y) && grid[y][x] == PATH) {
        entryPoint = {x, y};
        entrySet = true;
    } else {
        cout << "Invalid entry point! Must be a PATH cell." << endl;
    }
}

/**
 * @brief Sets the exit point if it's a valid PATH cell.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 */
 void Map::setExit(int x, int y) {
    if (isValidCoordinate(x, y) && grid[y][x] == PATH) {
        exitPoint = {x, y};
        exitSet = true;
    } else {
        cout << "Invalid exit point! Must be a PATH cell." << endl;
    }
}

/**
 * @brief Places a tower on the map if valid.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @return True if the tower was placed successfully, false otherwise.
 */
 bool Map::placeTower(int x, int y) {
    if (!isValidCoordinate(x, y)) {
        cout << "Invalid coordinates!" << endl;
        return false;
    }
    if (grid[y][x] == PATH) {
        cout << "Cannot place tower on a path!" << endl;
        return false;
    }
    if (grid[y][x] == TOWER) {
        cout << "A tower is already placed here!" << endl;
        return false;
    }

    grid[y][x] = TOWER;
    cout << "Tower placed at (" << x << ", " << y << ")" << endl;
    return true;
}

/**
 * @brief Returns the entry point coordinates.
 * @return Pair<int, int> containing (x,y) coordinates.
 */
 pair<int, int> Map::getEntry() const {
    return entryPoint;
}

/**
 * @brief Returns the exit point coordinates.
 * @return Pair<int, int> containing (x,y) coordinates.
 */
 pair<int, int> Map::getExit() const {
    return exitPoint;
}

/**
 * @brief Displays the map layout.
 */
 void Map::display() {
    cout << "Map Layout:\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (entrySet && entryPoint == make_pair(x, y))
                cout << "E ";          // Entry point
            else if (exitSet && exitPoint == make_pair(x, y))
                cout << "X ";          // Exit point
            else if (grid[y][x] == TOWER)
                cout << "T ";          // Tower cell (NEW)
            else if (grid[y][x] == PATH)
                cout << "# ";          // Path cell
            else
                cout << ". ";          // Scenery cell
        }
        cout << endl;
    }
}

/**
 * @brief Validates the map configuration.
 * @return True if map is valid, false otherwise.
 */
 bool Map::validateMap() {
    if (!entrySet || !exitSet) {
        cout << "Error: Entry and exit points must be set!" << endl;
        return false;
    }
    return isPathConnected();  // Check if there's a valid path
}

/**
 * @brief Checks if coordinates are within map boundaries.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @return True if coordinates are valid, false otherwise.
 */
bool Map::isValidCoordinate(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}


/**
 * @brief Checks if there is a valid path from the entry point to the exit point.
 *
 * This function uses a breadth-first search (BFS) algorithm to verify whether
 * a connected path exists from the entry point to the exit point.
 *
 * @return True if a valid path exists, false otherwise.
 */
bool Map::isPathConnected() {
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    queue<pair<int, int>> toCheck;

    // Start from entry point
    toCheck.push(entryPoint);
    visited[entryPoint.second][entryPoint.first] = true;

    while (!toCheck.empty()) {
        int currentX = toCheck.front().first;
        int currentY = toCheck.front().second;
        toCheck.pop();

        if (currentX == exitPoint.first && currentY == exitPoint.second) {
            return true;  // Path found!
        }

        // Check adjacent cells (up, down, left, right)
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        for (int i = 0; i < 4; i++) {
            int newX = currentX + dx[i];
            int newY = currentY + dy[i];

            if (isValidCoordinate(newX, newY) && !visited[newY][newX] && grid[newY][newX] == PATH) {
                toCheck.push({newX, newY});
                visited[newY][newX] = true;
            }
        }
    }
    return false;  // No path found to exit
}

/**
 * @brief Generates a random valid map layout.
 */
 void Map::generateRandomMap() {
    srand(time(0));  // Initialize random number generator

    // Set entry and exit points on opposite sides
    int entryX = 0, entryY = rand() % height;
    int exitX = width - 1, exitY = rand() % height;

    entryPoint = {entryX, entryY};
    exitPoint = {exitX, exitY};
    entrySet = exitSet = true;

    // Reset map to all scenery
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = SCENERY;
        }
    }

    // Create random path from entry to exit
    int x = entryX, y = entryY;
    grid[y][x] = PATH;

    while (x != exitX || y != exitY) {
        int direction = rand() % 2;  // Randomly choose horizontal or vertical movement
        if (direction == 0 && x != exitX) {
            x += (exitX > x) ? 1 : -1;  // Move right if exit is to the right, else left
        } else if (y != exitY) {
            y += (exitY > y) ? 1 : -1;  // Move down if exit is below, else up
        }
        grid[y][x] = PATH;  // Mark the cell as path
    }
}

/**
 * @brief Checks if a given cell is part of the PATH.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @return True if cell is PATH, false if SCENERY.
 */
bool Map::isPath(int x, int y) const {
    return isValidCoordinate(x, y) && grid[y][x] == PATH;
}
