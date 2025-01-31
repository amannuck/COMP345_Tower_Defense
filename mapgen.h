#ifndef MAPGEN_H
#define MAPGEN_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

enum CellType { SCENERY, PATH };

class Map {
private:
    int width, height;
    vector<vector<CellType>> grid;
    pair<int, int> entryPoint;
    pair<int, int> exitPoint;
    bool entrySet, exitSet;

    bool isValidCoordinate(int x, int y);
    bool isPathConnected();

public:
    Map(int w, int h);
    void setPath(int x, int y);
    void setEntry(int x, int y);
    void setExit(int x, int y);
    void display();
    bool validateMap();
    void generateRandomMap();
};

#endif // MAPGEN_H
