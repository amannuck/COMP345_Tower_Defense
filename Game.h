#pragma once
#include "Map.h"
#include <memory>
#include <vector>
#include "TowerManager.h"
#include "CritterWave.h"
#include "TowerObserver.h"

enum class GameState {
    MAP_SELECTION,
    PLAYING,
    GAME_OVER
};

class Game {
private:
    std::unique_ptr<TowerManager> towerManager;
    std::unique_ptr<CritterWave> currentWave;
    GameState state;
    Map* currentMap;
    std::vector<std::pair<int, int>> mapSizes;
    int selectedSize;
    int waveNumber;
    int playerCurrency;
    int lives; // Tracks how many critters can reach the end before game over

    void drawMapSelection() const;
    void handleMapSelection();
    void initializeMap();
    void startNewWave();
    void checkGameOver();
    std::string selectedTowerType;
    float towerMenuHeight = 100;
    float sideMenuWidth = 200;  // Width of the side menu for tower info

    Tower* selectedTower = nullptr;  // Currently selected tower

    void drawTowerMenu() const;
    void handleTowerMenuClick(Vector2 mousePos);

    // New methods for permanent side menu
    void drawSideMenu() const;
    void drawSideMenuDefault() const;
    void drawTowerInfoInSideMenu() const;

    void handleTowerSelection(Vector2 mousePos);
    void handleTowerInfoMenuClick(Vector2 mousePos);
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

//    void update();
    void update(float deltaTime);
    void draw() const;
    void restartGame();
    void placeTower(Vector2 mousePos);

};