#pragma once
#include "Map.h"

class MapEditor {
private:
    Map* currentMap;
    CellType currentTool;
    bool isDragging;

public:
    MapEditor(int width, int height);
    ~MapEditor();
    void update();
    void draw();
    Map* getMap() const { return currentMap; }
};