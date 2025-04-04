/**
 * @file TargetingStrategy.h
 * @brief Defines targeting strategy interfaces and implementations
 * @details This file implements the Strategy design pattern for tower targeting,
 *          allowing towers to select targets based on different criteria.
 */

#pragma once
#include <vector>
#include <memory>
#include "Critter.h"

/**
 * @class ITargetingStrategy
 * @brief Interface for all targeting strategies
 * @details Defines the common interface for various targeting strategies
 *          that towers can use to select their targets.
 */
class ITargetingStrategy {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~ITargetingStrategy() = default;
    
    /**
     * @brief Select a target from available critters
     * @param critters Vector of critters to choose from
     * @param towerPosition Position of the tower
     * @param range Maximum targeting range of the tower
     * @return Pointer to the selected critter, or nullptr if no valid target found
     */
    virtual Critter* selectTarget(const std::vector<Critter>& critters, 
                                 const Vector2& towerPosition, 
                                 float range) const = 0;
};

/**
 * @class FirstInRangeStrategy
 * @brief Strategy that selects the first critter that enters range
 * @details Simple strategy that returns the first active and alive critter
 *          found within the tower's range.
 */
class FirstInRangeStrategy : public ITargetingStrategy {
public:
    /**
     * @brief Select the first critter within range
     * @param critters Vector of critters to choose from
     * @param towerPosition Position of the tower
     * @param range Maximum targeting range of the tower
     * @return Pointer to the first critter in range, or nullptr if none found
     */
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};

/**
 * @class NearestCritterStrategy
 * @brief Strategy that selects the closest critter
 * @details Finds the critter with the minimum distance to the tower,
 *          prioritizing the closest target within range.
 */
class NearestCritterStrategy : public ITargetingStrategy {
public:
    /**
     * @brief Select the closest critter within range
     * @param critters Vector of critters to choose from
     * @param towerPosition Position of the tower
     * @param range Maximum targeting range of the tower
     * @return Pointer to the nearest critter in range, or nullptr if none found
     */
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};

/**
 * @class LowestHealthStrategy
 * @brief Strategy that selects the critter with the least health
 * @details Finds the critter with the minimum remaining health points,
 *          useful for finishing off weakened targets.
 */
class LowestHealthStrategy : public ITargetingStrategy {
public:
    /**
     * @brief Select the critter with the lowest health within range
     * @param critters Vector of critters to choose from
     * @param towerPosition Position of the tower
     * @param range Maximum targeting range of the tower
     * @return Pointer to the lowest health critter in range, or nullptr if none found
     */
    Critter* selectTarget(const std::vector<Critter>& critters, 
                         const Vector2& towerPosition, 
                         float range) const override;
};

/**
 * @class FarthestCritterStrategy
 * @brief Strategy that selects the farthest critter within range
 * @details Finds the critter with the maximum distance to the tower while
 *          still being within range, useful for long-range towers.
 */
class FarthestCritterStrategy : public ITargetingStrategy {
public:
    /**
     * @brief Select the farthest critter within range
     * @param critters Vector of critters to choose from
     * @param towerPosition Position of the tower
     * @param range Maximum targeting range of the tower
     * @return Pointer to the farthest critter in range, or nullptr if none found
     */
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};

/**
 * @class MostAdvancedStrategy
 * @brief Strategy that selects the critter furthest along the path
 * @details Finds the critter with the highest path index, targeting those
 *          closest to reaching the exit. This is a highly strategic targeting
 *          method for preventing critters from completing the path.
 */
class MostAdvancedStrategy : public ITargetingStrategy {
public:
    /**
     * @brief Select the critter that has progressed furthest along the path
     * @param critters Vector of critters to choose from
     * @param towerPosition Position of the tower
     * @param range Maximum targeting range of the tower
     * @return Pointer to the most advanced critter in range, or nullptr if none found
     */
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};
