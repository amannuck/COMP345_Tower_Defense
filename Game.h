#pragma once
#include "Map.h"
#include <memory>
#include <vector>
#include "TowerManager.h"

enum class GameState {
    MAP_SELECTION,
    PLAYING
};

class Game {
private:
    std::unique_ptr<TowerManager> towerManager;
    GameState state;
    Map* currentMap;
    std::vector<std::pair<int, int>> mapSizes;
    int selectedSize;

    void drawMapSelection() const;
    void handleMapSelection();
    void initializeMap();
    std::string selectedTowerType;
    float towerMenuHeight = 100;

    void drawTowerMenu() const;
    void handleTowerMenuClick(Vector2 mousePos);
    void updateTowers();
    void drawTowerShots() const;

    struct Shot {
        Vector2 start;
        Vector2 end;
        float timer;
        Color color;
    };
    std::vector<Shot> activeShots;

public:
    Game();
    ~Game();
    void update();
    void draw() const;
};