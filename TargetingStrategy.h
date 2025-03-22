// TargetingStrategy.h
#pragma once
#include <vector>
#include <memory>
#include "Critter.h"

// Interface for targeting strategies
class ITargetingStrategy {
public:
    virtual ~ITargetingStrategy() = default;
    
    // Select target method returns a pointer to the target critter (if found)
    virtual Critter* selectTarget(const std::vector<Critter>& critters, 
                                 const Vector2& towerPosition, 
                                 float range) const = 0;
};

// Strategy for selecting the first critter that enters the range
class FirstInRangeStrategy : public ITargetingStrategy {
public:
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};

// Strategy for selecting the nearest critter
class NearestCritterStrategy : public ITargetingStrategy {
public:
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};

// Strategy for selecting the critter with lowest health
class LowestHealthStrategy : public ITargetingStrategy {
public:
    Critter* selectTarget(const std::vector<Critter>& critters, 
                         const Vector2& towerPosition, 
                         float range) const override;
};

// Strategy for selecting the farthest critter
class FarthestCritterStrategy : public ITargetingStrategy {
public:
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};

// Strategy for selecting the most advanced critter on the path
class MostAdvancedStrategy : public ITargetingStrategy {
public:
    Critter* selectTarget(const std::vector<Critter>& critters,
                         const Vector2& towerPosition,
                         float range) const override;
};
