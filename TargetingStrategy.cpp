// TargetingStrategy.cpp
#include "TargetingStrategy.h"
#include "raymath.h"
#include <limits>
#include <algorithm>

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
                // Assuming the Critter class has a getPathIndex method
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
