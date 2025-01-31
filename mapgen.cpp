#include "mapgen.h"
#include <cstdlib>
#include <ctime>

Map::Map(int w, int h) {
    width = w;
    height = h;
    entrySet = false;
    exitSet = false;

    // Create a 2D grid filled with scenery
    grid.resize(height);
    for (int y = 0; y < height; y++) {
        grid[y].resize(width, SCENERY);
    }
}

void Map::setPath(int x, int y) {
    if (isValidCoordinate(x, y)) {
        grid[y][x] = PATH;
    }
}

void Map::setEntry(int x, int y) {
    if (isValidCoordinate(x, y) && grid[y][x] == PATH) {
        entryPoint = {x, y};
        entrySet = true;
    } else {
        cout << "Invalid entry point! Must be a PATH cell." << endl;
    }
}

void Map::setExit(int x, int y) {
    if (isValidCoordinate(x, y) && grid[y][x] == PATH) {
        exitPoint = {x, y};
        exitSet = true;
    } else {
        cout << "Invalid exit point! Must be a PATH cell." << endl;
    }
}

void Map::display() {
    cout << "Map Layout:\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (entrySet && entryPoint == make_pair(x, y))
                cout << "E ";  // Entry
            else if (exitSet && exitPoint == make_pair(x, y))
                cout << "X ";  // Exit
            else if (grid[y][x] == PATH)
                cout << "# ";  // Path
            else
                cout << ". ";  // Scenery
        }
        cout << endl;
    }
}

bool Map::validateMap() {
    if (!entrySet || !exitSet) {
        cout << "Error: Entry and exit points must be set!" << endl;
        return false;
    }
    return isPathConnected();
}

bool Map::isValidCoordinate(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Map::isPathConnected() {
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    queue<pair<int, int>> q;
    q.push(entryPoint);
    visited[entryPoint.second][entryPoint.first] = true;

    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == exitPoint.first && y == exitPoint.second) return true;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            if (isValidCoordinate(nx, ny) && !visited[ny][nx] && grid[ny][nx] == PATH) {
                visited[ny][nx] = true;
                q.push({nx, ny});
            }
        }
    }
    return false;
}

void Map::generateRandomMap() {
    srand(time(0)); //set random number generator using the current time

    int entryX = 0, entryY = rand() % height;
    int exitX = width - 1, exitY = rand() % height;

    entryPoint = {entryX, entryY};
    exitPoint = {exitX, exitY};
    entrySet = exitSet = true;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = SCENERY;
        }
    }

    int x = entryX, y = entryY;
    grid[y][x] = PATH;

    while (x != exitX || y != exitY) {
        int direction = rand() % 2;

        if (direction == 0 && x != exitX) {
            x += (exitX > x) ? 1 : -1;
        } else if (y != exitY) {
            y += (exitY > y) ? 1 : -1;
        }

        grid[y][x] = PATH;
    }
}
