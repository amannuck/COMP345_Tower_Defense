/**
 * @file TargetingStrategy.cpp
 * @brief Implementation of targeting strategy classes for tower targeting
 * @details This file implements various tower targeting strategies using the
 *          Strategy design pattern, allowing towers to select targets based on
 *          different criteria.
 */

#include "TargetingStrategy.h"
#include "raymath.h"
#include <limits>
#include <algorithm>

/**
 * @brief Selects the first critter within range
 * @param critters Vector of critters to choose from
 * @param towerPosition Position of the tower
 * @param range Maximum targeting range of the tower
 * @return Pointer to the first critter in range, or nullptr if none found
 * @details Iterates through critters and returns the first active and alive
 *          critter found within the tower's range.
 */
Critter* FirstInRangeStrategy::selectTarget(const std::vector<Critter>& critters,
                                           const Vector2& towerPosition,
                                           float range) const {
    // Simply returns the first critter found in range
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(towerPosition, critter.getPosition());
            if (distance <= range) {
                return const_cast<Critter*>(&critter);
            }
        }
    }
    return nullptr;
}

/**
 * @brief Selects the closest critter within range
 * @param critters Vector of critters to choose from
 * @param towerPosition Position of the tower
 * @param range Maximum targeting range of the tower
 * @return Pointer to the nearest critter in range, or nullptr if none found
 * @details Finds the critter with the minimum distance to the tower,
 *          prioritizing the closest target within range.
 */
Critter* NearestCritterStrategy::selectTarget(const std::vector<Critter>& critters,
                                             const Vector2& towerPosition,
                                             float range) const {
    // Find the critter with minimum distance to tower
    Critter* target = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(towerPosition, critter.getPosition());
            if (distance <= range && distance < minDistance) {
                minDistance = distance;
                target = const_cast<Critter*>(&critter);
            }
        }
    }
    
    return target;
}

/**
 * @brief Selects the critter with the lowest health within range
 * @param critters Vector of critters to choose from
 * @param towerPosition Position of the tower
 * @param range Maximum targeting range of the tower
 * @return Pointer to the lowest health critter in range, or nullptr if none found
 * @details Finds the critter with the minimum remaining health points,
 *          useful for finishing off weakened targets.
 */
Critter* LowestHealthStrategy::selectTarget(const std::vector<Critter>& critters,
                                          const Vector2& towerPosition,
                                          float range) const {
    // Find the critter with lowest health points
    Critter* target = nullptr;
    float lowestHealth = std::numeric_limits<float>::max();
    
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(towerPosition, critter.getPosition());
            if (distance <= range) {
                float health = critter.getHitPoints();
                if (health < lowestHealth) {
                    lowestHealth = health;
                    target = const_cast<Critter*>(&critter);
                }
            }
        }
    }
    
    return target;
}

/**
 * @brief Selects the farthest critter within range
 * @param critters Vector of critters to choose from
 * @param towerPosition Position of the tower
 * @param range Maximum targeting range of the tower
 * @return Pointer to the farthest critter in range, or nullptr if none found
 * @details Finds the critter with the maximum distance to the tower while
 *          still being within range, useful for towers with long range.
 */
Critter* FarthestCritterStrategy::selectTarget(const std::vector<Critter>& critters,
                                             const Vector2& towerPosition,
                                             float range) const {
    // Find the critter with maximum distance (but still in range)
    Critter* target = nullptr;
    float maxDistance = 0.0f;
    
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(towerPosition, critter.getPosition());
            if (distance <= range && distance > maxDistance) {
                maxDistance = distance;
                target = const_cast<Critter*>(&critter);
            }
        }
    }
    
    return target;
}

/**
 * @brief Selects the critter that has progressed furthest along the path
 * @param critters Vector of critters to choose from
 * @param towerPosition Position of the tower
 * @param range Maximum targeting range of the tower
 * @return Pointer to the most advanced critter in range, or nullptr if none found
 * @details Finds the critter with the highest path index, targeting those
 *          closest to reaching the exit. This is a highly strategic targeting
 *          method for preventing critters from completing the path.
 */
Critter* MostAdvancedStrategy::selectTarget(const std::vector<Critter>& critters,
                                           const Vector2& towerPosition,
                                           float range) const {
    // Find the critter that is furthest along the path
    Critter* target = nullptr;
    int highestPathIndex = -1;
    
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(towerPosition, critter.getPosition());
            if (distance <= range) {
                // Get the critter's position on the path
                int pathIndex = critter.getPathIndex();
                if (pathIndex > highestPathIndex) {
                    highestPathIndex = pathIndex;
                    target = const_cast<Critter*>(&critter);
                }
            }
        }
    }
    
    return target;
}
