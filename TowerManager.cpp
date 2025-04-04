/**
 * @file TowerManager.cpp
 * @brief Implementation of the TowerManager class with decorator support
 * @details This file provides the implementation for the TowerManager class,
 *          which handles tower creation, purchasing, upgrading, and selling.
 *          It also includes support for applying decorators to existing towers.
 */

#include "TowerManager.h"
#include <algorithm>

/**
 * @brief Constructor for the TowerManager class
 * @param initialCurrency Starting amount of player currency
 */
TowerManager::TowerManager(int initialCurrency) : playerCurrency(initialCurrency) {}

/**
 * @brief Checks if the player can afford a specific tower type
 * @param towerType String identifier for the tower type
 * @return true if the player has enough currency, false otherwise
 */
bool TowerManager::canAffordTower(const std::string& towerType) const {
    auto tower = createTower(towerType);
    return tower && playerCurrency >= tower->getBuyCost();
}

/**
 * @brief Checks if the player can afford to upgrade a tower
 * @param tower Pointer to the tower to upgrade
 * @return true if the player has enough currency, false otherwise
 */
bool TowerManager::canAffordUpgrade(const ITower* tower) const {
    return tower && playerCurrency >= tower->getUpgradeCost();
}

/**
 * @brief Purchases and places a new tower
 * @param towerType String identifier for the tower type
 * @param position Position vector where the tower should be placed
 * @return true if the purchase was successful, false if the player can't afford it
 * @details Creates a tower of the specified type, deducts the cost from the player's
 *          currency, sets the tower's position, and adds it to the collection.
 */
bool TowerManager::buyTower(const std::string& towerType, Vector2 position) {
    auto tower = createTower(towerType);
    if (!tower || playerCurrency < tower->getBuyCost()) return false;
    
    playerCurrency -= tower->getBuyCost();
    tower->setPosition(position);
    towers.push_back(std::move(tower));
    return true;
}

/**
 * @brief Upgrades an existing tower
 * @param tower Pointer to the tower to upgrade
 * @return true if the upgrade was successful, false if the player can't afford it
 * @details Checks if the player can afford the upgrade, deducts the cost,
 *          and calls the tower's upgrade method to improve its capabilities.
 */
bool TowerManager::upgradeTower(ITower* tower) {
    if (!tower || playerCurrency < tower->getUpgradeCost()) return false;
    
    playerCurrency -= tower->getUpgradeCost();
    tower->upgrade();
    return true;
}

/**
 * @brief Sells a tower and refunds part of its cost
 * @param tower Pointer to the tower to sell
 * @return The amount of currency refunded
 * @details Calculates the refund value, adds it to the player's currency,
 *          and removes the tower from the collection.
 */
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

/**
 * @brief Creates a new tower of the specified type
 * @param towerType String identifier for the tower type
 * @return Unique pointer to the created tower, or nullptr if type is invalid
 * @details Factory method that creates different tower types based on the string identifier.
 */
std::unique_ptr<ITower> TowerManager::createTower(const std::string& towerType) const {
    std::unique_ptr<ITower> baseTower;
    
    if (towerType == "Basic") baseTower = std::make_unique<BasicTower>();
    else if (towerType == "Area") baseTower = std::make_unique<AreaTower>();
    else if (towerType == "Slow") baseTower = std::make_unique<SlowTower>();
    else if (towerType == "Sniper") baseTower = std::make_unique<SniperTower>();
    else return nullptr;
    
    return baseTower;
}

/**
 * @brief Adds splash damage effect to a tower
 * @param tower Unique pointer to the tower to enhance
 * @return Unique pointer to the decorated tower
 * @details Helper method that wraps a tower with the SplashDecorator.
 */
std::unique_ptr<ITower> TowerManager::addSplashEffect(std::unique_ptr<ITower> tower) {
    return std::make_unique<SplashDecorator>(std::move(tower));
}

/**
 * @brief Adds burning effect to a tower
 * @param tower Unique pointer to the tower to enhance
 * @return Unique pointer to the decorated tower
 * @details Helper method that wraps a tower with the BurningDecorator.
 */
std::unique_ptr<ITower> TowerManager::addBurningEffect(std::unique_ptr<ITower> tower) {
    return std::make_unique<BurningDecorator>(std::move(tower));
}

/**
 * @brief Adds freezing effect to a tower
 * @param tower Unique pointer to the tower to enhance
 * @return Unique pointer to the decorated tower
 * @details Helper method that wraps a tower with the FreezingDecorator.
 */
std::unique_ptr<ITower> TowerManager::addFreezingEffect(std::unique_ptr<ITower> tower) {
    return std::make_unique<FreezingDecorator>(std::move(tower));
}

/**
 * @brief Upgrades a tower by adding a decorator effect
 * @param tower Pointer to the tower to upgrade
 * @param decoratorType String identifier for the decorator type
 * @return true if the upgrade was successful, false otherwise
 * @details Finds the tower in the collection, applies the specified decorator,
 *          and charges the player a premium (1.5x) upgrade cost for the enhancement.
 */
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
