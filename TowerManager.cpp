//
// Created by amann on 23/02/2025.
//

#include "TowerManager.h"
#include <algorithm>

TowerManager::TowerManager(int initialCurrency) : playerCurrency(initialCurrency) {}

bool TowerManager::canAffordTower(const std::string& towerType) const {
    auto tower = createTower(towerType);
    return tower && playerCurrency >= tower->getBuyCost();
}

bool TowerManager::canAffordUpgrade(const Tower* tower) const {
    return tower && playerCurrency >= tower->getUpgradeCost();
}

bool TowerManager::buyTower(const std::string& towerType, Vector2 position) {
    auto tower = createTower(towerType);
    if (!tower || playerCurrency < tower->getBuyCost()) return false;

    playerCurrency -= tower->getBuyCost();
    tower->setPosition(position);
    towers.push_back(std::move(tower));
    return true;
}

bool TowerManager::upgradeTower(Tower* tower) {
    if (!tower || playerCurrency < tower->getUpgradeCost()) return false;

    playerCurrency -= tower->getUpgradeCost();
    tower->upgrade();
    return true;
}

int TowerManager::sellTower(Tower* tower) {
    if (!tower) return 0;

    int refundValue = tower->getRefundValue();
    playerCurrency += refundValue;

    // Remove the tower from the vector
    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [tower](const std::unique_ptr<Tower>& t) { return t.get() == tower; }),
        towers.end()
    );

    return refundValue;
}

std::unique_ptr<Tower> TowerManager::createTower(const std::string& towerType) const {
    if (towerType == "Basic") return std::make_unique<BasicTower>();
    if (towerType == "Area") return std::make_unique<AreaTower>();
    if (towerType == "Slow") return std::make_unique<SlowTower>();
    return nullptr;
}