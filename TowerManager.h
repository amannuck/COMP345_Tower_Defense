/**
 * @file TowerManager.h
 * @brief Defines the TowerManager class for handling tower operations
 * @details This class manages tower creation, purchasing, upgrading, and selling,
 *          as well as player currency and tower enhancement through decorators.
 */

#pragma once
#include <vector>
#include <memory>
#include "Tower.h"

/**
 * @class TowerManager
 * @brief Manages towers and player currency in the tower defense game
 * @details Provides methods for tower lifecycle management, decorator application,
 *          and currency tracking.
 */
class TowerManager {
private:
    std::vector<std::unique_ptr<ITower>> towers;  ///< Collection of all active towers
    int playerCurrency;                           ///< Current player currency amount
    
    /**
     * @brief Helper method to add splash damage effect to a tower
     * @param tower Unique pointer to the tower to enhance
     * @return Unique pointer to the decorated tower
     */
    std::unique_ptr<ITower> addSplashEffect(std::unique_ptr<ITower> tower);
    
    /**
     * @brief Helper method to add burning effect to a tower
     * @param tower Unique pointer to the tower to enhance
     * @return Unique pointer to the decorated tower
     */
    std::unique_ptr<ITower> addBurningEffect(std::unique_ptr<ITower> tower);
    
    /**
     * @brief Helper method to add freezing effect to a tower
     * @param tower Unique pointer to the tower to enhance
     * @return Unique pointer to the decorated tower
     */
    std::unique_ptr<ITower> addFreezingEffect(std::unique_ptr<ITower> tower);
    
public:
    /**
     * @brief Constructor for TowerManager
     * @param initialCurrency Starting amount of player currency (default: 100)
     */
    TowerManager(int initialCurrency = 100);
    
    /**
     * @brief Checks if the player can afford a specific tower type
     * @param towerType String identifier for the tower type
     * @return true if the player has enough currency, false otherwise
     */
    bool canAffordTower(const std::string& towerType) const;
    
    /**
     * @brief Checks if the player can afford to upgrade a tower
     * @param tower Pointer to the tower to upgrade
     * @return true if the player has enough currency, false otherwise
     */
    bool canAffordUpgrade(const ITower* tower) const;
    
    /**
     * @brief Purchases and places a new tower
     * @param towerType String identifier for the tower type
     * @param position Position vector where the tower should be placed
     * @return true if the purchase was successful, false if the player can't afford it
     */
    bool buyTower(const std::string& towerType, Vector2 position);
    
    /**
     * @brief Upgrades an existing tower
     * @param tower Pointer to the tower to upgrade
     * @return true if the upgrade was successful, false if the player can't afford it
     */
    bool upgradeTower(ITower* tower);
    
    /**
     * @brief Sells a tower and refunds part of its cost
     * @param tower Pointer to the tower to sell
     * @return The amount of currency refunded
     */
    int sellTower(ITower* tower);
    
    /**
     * @brief Upgrades a tower by adding a decorator effect
     * @param tower Pointer to the tower to upgrade
     * @param decoratorType String identifier for the decorator type
     * @return true if the upgrade was successful, false otherwise
     * @details Applies a decorator to enhance the tower with special effects.
     *          Costs 1.5 times the normal upgrade cost.
     */
    bool upgradeWithDecorator(ITower* tower, const std::string& decoratorType);
    
    /**
     * @brief Gets the collection of all active towers
     * @return Constant reference to the vector of tower pointers
     */
    const std::vector<std::unique_ptr<ITower>>& getTowers() const { return towers; }
    
    /**
     * @brief Gets the current player currency amount
     * @return Current currency amount
     */
    int getCurrency() const { return playerCurrency; }
    
    /**
     * @brief Adds (or subtracts) currency from the player
     * @param thiscurr Amount to add (can be negative to subtract)
     */
    void addCurrency(int thiscurr) { playerCurrency = thiscurr + playerCurrency; }
    
    /**
     * @brief Creates a new tower of the specified type
     * @param towerType String identifier for the tower type
     * @return Unique pointer to the created tower, or nullptr if type is invalid
     */
    std::unique_ptr<ITower> createTower(const std::string& towerType) const;
};
