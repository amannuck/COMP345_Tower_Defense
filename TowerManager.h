#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Tower.h"
#include "TowerObserver.h"

class TowerManager : public ITowerObserver {
private:
    std::vector<std::unique_ptr<Tower>> towers;
    int playerCurrency;

public:
    TowerManager(int initialCurrency = 100);
    ~TowerManager() = default;

    bool canAffordTower(const std::string& towerType) const;
    bool canAffordUpgrade(const Tower* tower) const;
    bool buyTower(const std::string& towerType, Vector2 position);
    bool upgradeTower(Tower* tower);
    int sellTower(Tower* tower);

    const std::vector<std::unique_ptr<Tower>>& getTowers() const { return towers; }
    int getCurrency() const { return playerCurrency; }

    // Observer method implementation
    void onTowerEvent(Tower* tower, TowerEventType eventType) override;

    // Factory methods
    std::unique_ptr<Tower> createTower(const std::string& towerType) const;
};