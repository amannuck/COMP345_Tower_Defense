#include "mapgen.h"
#include <cstdlib>  // rand()
#include <ctime>    // time()

// Constructor: Initialize map with given dimensions
Map::Map(int w, int h) {
    width = w;
    height = h;
    // No entry and exit point set initially
    entrySet = false;
    exitSet = false;

    // Create a 2D grid filled with scenery
    grid.resize(height);
    for (int y = 0; y < height; y++) {
        grid[y].resize(width, SCENERY);  // All cells start as SCENERY
    }
}

// Sets a cell as PATH if coordinates are valid
void Map::setPath(int x, int y) {
    if (isValidCoordinate(x, y)) {
        grid[y][x] = PATH;
    }
}

// Sets the entry point if it's a valid PATH cell
void Map::setEntry(int x, int y) {
    if (isValidCoordinate(x, y) && grid[y][x] == PATH) {
        entryPoint = {x, y};
        entrySet = true;
    } else {
        cout << "Invalid entry point! Must be a PATH cell." << endl;
    }
}

// Sets the exit point if it's a valid PATH cell
void Map::setExit(int x, int y) {
    if (isValidCoordinate(x, y) && grid[y][x] == PATH) {
        exitPoint = {x, y};
        exitSet = true;
    } else {
        cout << "Invalid exit point! Must be a PATH cell." << endl;
    }
}

// Displays the map
void Map::display() {
    cout << "Map Layout:\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (entrySet && entryPoint == make_pair(x, y))
                cout << "E ";          // Entry point
            else if (exitSet && exitPoint == make_pair(x, y))
                cout << "X ";          // Exit point
            else if (grid[y][x] == PATH)
                cout << "# ";          // Path cell
            else
                cout << ". ";          // Scenery cell
        }
        cout << endl;
    }
}

// Validates the map configuration
bool Map::validateMap() {
    if (!entrySet || !exitSet) {
        cout << "Error: Entry and exit points must be set!" << endl;
        return false;
    }
    return isPathConnected();  // Check if there's a valid path
}

// Checks if coordinates are within map boundaries
bool Map::isValidCoordinate(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Checks if there's a path from entry to exit
bool Map::isPathConnected() {
    // Create a 2D array to keep track of visited cells
    vector<vector<bool>> visited;
    visited.resize(height);
    for (int i = 0; i < height; i++) {
        visited[i].resize(width, false);
    }

    // Create a queue
    queue<pair<int, int>> toCheck;

    // Start from entry point
    toCheck.push(entryPoint);
    visited[entryPoint.second][entryPoint.first] = true;

    while (!toCheck.empty()) {
        // Get the current position
        int currentX = toCheck.front().first;
        int currentY = toCheck.front().second;
        toCheck.pop();

        // Check if we reached the exit
        if (currentX == exitPoint.first && currentY == exitPoint.second) {
            return true;  // Path found!
        }

        // Check cell above
        if (isValidCoordinate(currentX, currentY - 1) &&
            !visited[currentY - 1][currentX] &&
            grid[currentY - 1][currentX] == PATH)
        {
            toCheck.push({currentX, currentY - 1});
            visited[currentY - 1][currentX] = true;
        }

        // Check cell below
        if (isValidCoordinate(currentX, currentY + 1) &&
            !visited[currentY + 1][currentX] &&
            grid[currentY + 1][currentX] == PATH)
        {
            toCheck.push({currentX, currentY + 1});
            visited[currentY + 1][currentX] = true;
        }

        // Check cell to the left
        if (isValidCoordinate(currentX - 1, currentY) &&
            !visited[currentY][currentX - 1] &&
            grid[currentY][currentX - 1] == PATH)
        {
            toCheck.push({currentX - 1, currentY});
            visited[currentY][currentX - 1] = true;
        }

        // Check cell to the right
        if (isValidCoordinate(currentX + 1, currentY) &&
            !visited[currentY][currentX + 1] &&
            grid[currentY][currentX + 1] == PATH)
        {
            toCheck.push({currentX + 1, currentY});
            visited[currentY][currentX + 1] = true;
        }
    }

    return false;  // No path found to exit
}

// Generates a random valid map
void Map::generateRandomMap() {
    srand(time(0));  // Initialize random number generator with current time

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

        // Move towards exit
        if (direction == 0 && x != exitX) {
            x += (exitX > x) ? 1 : -1;  // Move right if exit is to the right, else left
        } else if (y != exitY) {
            y += (exitY > y) ? 1 : -1;  // Move down if exit is below, else up
        }

        grid[y][x] = PATH;  // Mark the cell as path
    }
}