// Map.h
#pragma once
#include <vector>
#include <queue>
#include <raylib.h>
#include <cstdlib>
#include <ctime>

enum class CellType {
    PATH,
    SCENERY,
    ENTRY,
    EXIT,
    TOWER
};

class Map {
private:
    int width;
    int height;
    std::vector<std::vector<CellType>> grid;
    Vector2 entryPoint;
    Vector2 exitPoint;
    int CELL_SIZE;  // Dynamic cell size based on map dimensions

    bool isValidCoordinate(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    bool hasValidPath() const;

public:
    Map(int w, int h);
    void generateRandomMap();
    CellType getCellType(int x, int y) const;
    bool placeTower(int x, int y);
    void draw(int offsetX = 0, int offsetY = 0, int cellSize = 0) const;
    Vector2 getEntryPoint() const { return entryPoint; }
    Vector2 getExitPoint() const { return exitPoint; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool validateMap() const;
    void setCellType(int x, int y, CellType type);
};