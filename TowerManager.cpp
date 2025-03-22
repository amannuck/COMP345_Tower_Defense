// TowerManager.cpp - Add decorator support
#include "TowerManager.h"
#include <algorithm>

TowerManager::TowerManager(int initialCurrency) : playerCurrency(initialCurrency) {}

bool TowerManager::canAffordTower(const std::string& towerType) const {
    auto tower = createTower(towerType);
    return tower && playerCurrency >= tower->getBuyCost();
}

bool TowerManager::canAffordUpgrade(const ITower* tower) const {
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

bool TowerManager::upgradeTower(ITower* tower) {
    if (!tower || playerCurrency < tower->getUpgradeCost()) return false;

    playerCurrency -= tower->getUpgradeCost();
    tower->upgrade();
    return true;
}

int TowerManager::sellTower(ITower* tower) {
    if (!tower) return 0;

    int refundValue = tower->getRefundValue();
    playerCurrency += refundValue;

    // Remove the tower from the vector
    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [tower](const std::unique_ptr<ITower>& t) { return t.get() == tower; }),
        towers.end()
    );

    return refundValue;
}

std::unique_ptr<ITower> TowerManager::createTower(const std::string& towerType) const {
    std::unique_ptr<ITower> baseTower;
    
    if (towerType == "Basic") baseTower = std::make_unique<BasicTower>();
    else if (towerType == "Area") baseTower = std::make_unique<AreaTower>();
    else if (towerType == "Slow") baseTower = std::make_unique<SlowTower>();
    else if (towerType == "Sniper") baseTower = std::make_unique<SniperTower>();
    else return nullptr;
    
    return baseTower;
}

// Helper methods for adding decorators
std::unique_ptr<ITower> TowerManager::addSplashEffect(std::unique_ptr<ITower> tower) {
    return std::make_unique<SplashDecorator>(std::move(tower));
}

std::unique_ptr<ITower> TowerManager::addBurningEffect(std::unique_ptr<ITower> tower) {
    return std::make_unique<BurningDecorator>(std::move(tower));
}

std::unique_ptr<ITower> TowerManager::addFreezingEffect(std::unique_ptr<ITower> tower) {
    return std::make_unique<FreezingDecorator>(std::move(tower));
}

// Method to apply decorator when upgrading
bool TowerManager::upgradeWithDecorator(ITower* tower, const std::string& decoratorType) {
    if (!tower || playerCurrency < tower->getUpgradeCost() * 1.5) return false;
    
    // Find the tower in our collection
    auto it = std::find_if(towers.begin(), towers.end(),
        [tower](const std::unique_ptr<ITower>& t) { return t.get() == tower; });
    
    if (it == towers.end()) return false;
    
    // Get the current tower and take it out of the collection
    std::unique_ptr<ITower> currentTower = std::move(*it);
    towers.erase(it);
    
    // Apply the decorator
    std::unique_ptr<ITower> decoratedTower;
    
    if (decoratorType == "Splash") {
        decoratedTower = addSplashEffect(std::move(currentTower));
    }
    else if (decoratorType == "Burning") {
        decoratedTower = addBurningEffect(std::move(currentTower));
    }
    else if (decoratorType == "Freezing") {
        decoratedTower = addFreezingEffect(std::move(currentTower));
    }
    else {
        // Decorator type not recognized, put the tower back and return false
        towers.push_back(std::move(currentTower));
        return false;
    }
    
    // Charge the player and add the decorated tower back to our collection
    playerCurrency -= tower->getUpgradeCost() * 1.5;
    towers.push_back(std::move(decoratedTower));
    
    return true;
}
