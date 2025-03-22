// Tower.h
#pragma once
#include <raylib.h>
#include <string>
#include <memory>

#include "Critter.h"
#include "TargetingStrategy.h"

class Tower {
protected:
    int level;
    float range;
    float power;
    float fireRate;  // Shots per second
    float lastShotTime;
    Vector2 position;
    int buyCost;
    int upgradeCost;
    std::string name;
    Color color;
    
    // Add targeting strategy
    std::unique_ptr<ITargetingStrategy> targetingStrategy;

public:
    Tower(float range, float power, float fireRate, int buyCost, int upgradeCost, 
          const std::string& name, const Color& color);
    virtual ~Tower();

    virtual void upgrade();
    virtual void attackCritters(std::vector<Critter>& critters);
    virtual int getRefundValue() const;
    virtual void draw() const;
    
    // Method to set targeting strategy
    void setTargetingStrategy(std::unique_ptr<ITargetingStrategy> strategy) {
        targetingStrategy = std::move(strategy);
    }
    
    // Getters
    float getRange() const { return range; }
    float getPower() const { return power; }
    float getFireRate() const { return fireRate; }
    int getBuyCost() const { return buyCost; }
    int getUpgradeCost() const { return upgradeCost; }
    int getLevel() const { return level; }
    const std::string& getName() const { return name; }
    Vector2 getPosition() const { return position; }
    
    void setPosition(Vector2 pos) { position = pos; }
    bool canShoot() const;
    void resetShotTimer();
};

class BasicTower : public Tower {
public:
    BasicTower();
    void upgrade() override;
};

class AreaTower : public Tower {
private:
    float areaRadius;
public:
    AreaTower();
    void upgrade() override;
    void attackCritters(std::vector<Critter>& critters) override;
    float getAreaRadius() const { return areaRadius; }
};

class SlowTower : public Tower {
private:
    float slowEffect;  // Percentage to slow enemies
    float slowDuration;  // Duration of slow effect in seconds
public:
    SlowTower();
    void upgrade() override;
    void attackCritters(std::vector<Critter>& critters) override;
    float getSlowEffect() const { return slowEffect; }
    float getSlowDuration() const { return slowDuration; }
};

// New Sniper Tower class
class SniperTower : public Tower {
private:
    float criticalChance;  // Chance for critical hit
    float criticalMultiplier;  // Damage multiplier for critical hits
public:
    SniperTower();
    void upgrade() override;
    void attackCritters(std::vector<Critter>& critters) override;
    float getCriticalChance() const { return criticalChance; }
    float getCriticalMultiplier() const { return criticalMultiplier; }
};
