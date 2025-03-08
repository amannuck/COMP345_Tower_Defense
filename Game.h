#pragma once
#include "Map.h"
#include <memory>
#include <vector>

#include "CritterWave.h"
#include "TowerManager.h"
#include "CritterObserver.h"

enum class GameState {
    MAP_SELECTION,
    PLAYING
};

class Game : public CritterObserver { 
private:
    std::unique_ptr<TowerManager> towerManager;
    GameState state;
    Map* currentMap;
    std::vector<std::pair<int, int>> mapSizes;
    int selectedSize;

    std::unique_ptr<CritterWave> critterWave;


    void drawMapSelection() const;
    void handleMapSelection();
    void initializeMap();
    std::string selectedTowerType;
    float towerMenuHeight = 100;
    float sideMenuWidth = 200;  // Width of the side menu for tower info

    Tower* selectedTower = nullptr;  // Currently selected tower

    void drawTowerMenu() const;
    void handleTowerMenuClick(Vector2 mousePos);

    // New methods for permanent side menu
    void drawSideMenu() const;

    void handleSideMenuButtonClick(Vector2 mousePos);

    void drawSideMenuButton() const;
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

    struct RewardNotification {
        Vector2 position;
        int amount;
        float timer;
    };
    std::vector<RewardNotification> rewardNotifications;    

public:
    Game();
    ~Game();
    void update();
    void draw() const;
    
    // CritterObserver interface implementation
    void onCritterReachedEnd(const Critter& critter) override;
    void onCritterDefeated(const Critter& critter) override;
    void addRewardNotification(const Vector2& position, int amount);
};