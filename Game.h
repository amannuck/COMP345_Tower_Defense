#pragma once
#include "Map.h"
#include <memory>
#include <vector>
#include "CritterWave.h"
#include "TowerManager.h"
#include "CritterObserver.h"
#include "MapEditor.h"  // Include MapEditor header

enum class GameState {
    MAP_SELECTION,
    MAP_EDITING,
    PLAYING
};

class Game : public CritterObserver {
private:
    std::unique_ptr<TowerManager> towerManager;
    GameState state;
    Map* currentMap;
    std::vector<std::pair<int, int>> mapSizes;
    int selectedSize;
    std::string widthInput;  // Stores the width input as a string
    std::string heightInput; // Stores the height input as a string
    bool isEditingWidth;

    std::unique_ptr<CritterWave> critterWave;
    std::unique_ptr<MapEditor> mapEditor;  // MapEditor instance
    
    int currentWave = 0;  // Track the current wave number

    void drawMapSelection() const;
    void handleMapSelection();
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