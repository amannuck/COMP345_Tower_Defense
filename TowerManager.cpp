#include "TowerManager.h"
#include <algorithm>
#include <iostream>

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
    
    // Register as observer before setting position to catch the TOWER_PLACED event
    tower->addObserver(this);
    
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
    
    // Notify observers before removing the tower
    tower->notifyObservers(TowerEventType::TOWER_SOLD);

    // Remove the tower from the vector
    towers.erase(
            std::remove_if(towers.begin(), towers.end(),
                           [tower](const std::unique_ptr<Tower>& t) { return t.get() == tower; }),
            towers.end()
    );

    return refundValue;
}

std::unique_ptr<Tower> TowerManager::createTower(const std::string& towerType) const {
    std::unique_ptr<Tower> tower = nullptr;
    
    if (towerType == "Basic") tower = std::make_unique<BasicTower>();
    else if (towerType == "Area") tower = std::make_unique<AreaTower>();
    else if (towerType == "Slow") tower = std::make_unique<SlowTower>();
    
    return tower;
}

// Observer pattern implementation
void TowerManager::onTowerEvent(Tower* tower, TowerEventType eventType) {
    if (!tower) return;
    
    switch (eventType) {
        case TowerEventType::TOWER_FIRED:
            // Handle tower firing event
            std::cout << "Tower " << tower->getName() << " fired!" << std::endl;
            break;
            
        case TowerEventType::TOWER_UPGRADED:
            // Handle tower upgrade event
            std::cout << "Tower " << tower->getName() << " upgraded to level " 
                      << tower->getLevel() << "!" << std::endl;
            break;
            
        case TowerEventType::TOWER_SOLD:
            // Handle tower sold event
            std::cout << "Tower " << tower->getName() << " was sold!" << std::endl;
            break;
            
        case TowerEventType::TOWER_PLACED:
            // Handle tower placement event
            std::cout << "New " << tower->getName() << " placed at position (" 
                      << tower->getPosition().x << ", " << tower->getPosition().y << ")!" << std::endl;
            break;
    }
}