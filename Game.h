#pragma once
#include "Map.h"
#include <string>

enum class GameState {
    MAP_SELECTION,
    PLAYING
};

class Game {
private:
    GameState state;
    Map* currentMap;
    std::vector<std::pair<int, int>> mapSizes;
    int selectedSize;

    void drawMapSelection() const;
    void handleMapSelection();
    void initializeMap();

public:
    Game();
    ~Game();
    void update();
    void draw() const;
};