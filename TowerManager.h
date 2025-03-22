// TowerManager.h - Add decorator support
#pragma once
#include <vector>
#include <memory>
#include "Tower.h"

class TowerManager {
private:
    std::vector<std::unique_ptr<ITower>> towers;
    int playerCurrency;

    // Helper methods for adding decorators
    std::unique_ptr<ITower> addSplashEffect(std::unique_ptr<ITower> tower);
    std::unique_ptr<ITower> addBurningEffect(std::unique_ptr<ITower> tower);
    std::unique_ptr<ITower> addFreezingEffect(std::unique_ptr<ITower> tower);

public:
    TowerManager(int initialCurrency = 100);

    bool canAffordTower(const std::string& towerType) const;
    bool canAffordUpgrade(const ITower* tower) const;
    bool buyTower(const std::string& towerType, Vector2 position);
    bool upgradeTower(ITower* tower);
    int sellTower(ITower* tower);
    
    // New method to apply a decorator when upgrading
    bool upgradeWithDecorator(ITower* tower, const std::string& decoratorType);

    const std::vector<std::unique_ptr<ITower>>& getTowers() const { return towers; }
    int getCurrency() const { return playerCurrency; }
    void addCurrency(int thiscurr) { playerCurrency = thiscurr + playerCurrency; }

    // Factory methods
    std::unique_ptr<ITower> createTower(const std::string& towerType) const;
};
