// Tower.h - Decorator Pattern Implementation
#pragma once
#include <raylib.h>
#include <string>
#include <memory>

#include "Critter.h"
#include "TargetingStrategy.h"

// Abstract Base Tower Component
class ITower {
public:
    virtual ~ITower() = default;
    
    // Core methods that all towers must implement
    virtual void upgrade() = 0;
    virtual void attackCritters(std::vector<Critter>& critters) = 0;
    virtual int getRefundValue() const = 0;
    virtual void draw() const = 0;
    
    // Getters
    virtual float getRange() const = 0;
    virtual float getPower() const = 0;
    virtual float getFireRate() const = 0;
    virtual int getBuyCost() const = 0;
    virtual int getUpgradeCost() const = 0;
    virtual int getLevel() const = 0;
    virtual const std::string& getName() const = 0;
    virtual Vector2 getPosition() const = 0;
    
    // Setters
    virtual void setPosition(Vector2 pos) = 0;
    virtual bool canShoot() const = 0;
    virtual void resetShotTimer() = 0;
    virtual void setTargetingStrategy(std::unique_ptr<ITargetingStrategy> strategy) = 0;
};

// Concrete Component - Basic Tower implementation
class Tower : public ITower {
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

    void upgrade() override;
    void attackCritters(std::vector<Critter>& critters) override;
    int getRefundValue() const override;
    void draw() const override;
    
    // Method to set targeting strategy
    void setTargetingStrategy(std::unique_ptr<ITargetingStrategy> strategy) override {
        targetingStrategy = std::move(strategy);
    }
    
    // Getters
    float getRange() const override { return range; }
    float getPower() const override { return power; }
    float getFireRate() const override { return fireRate; }
    int getBuyCost() const override { return buyCost; }
    int getUpgradeCost() const override { return upgradeCost; }
    int getLevel() const override { return level; }
    const std::string& getName() const override { return name; }
    Vector2 getPosition() const override { return position; }
    
    void setPosition(Vector2 pos) override { position = pos; }
    bool canShoot() const override;
    void resetShotTimer() override;


};

void UpdateTowerShots(float deltaTime);
void DrawTowerShots();

struct TowerShot {
    Vector2 start;
    Vector2 end;
    Vector2 currentPos;
    float speed;
    Color color;
    float timer;
    bool hit;
};

// Concrete Towers - These remain mostly the same
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

// Base Decorator
class TowerDecorator : public ITower {
protected:
    std::unique_ptr<ITower> wrappedTower;
    
public:
    TowerDecorator(std::unique_ptr<ITower> tower) : wrappedTower(std::move(tower)) {}
    
    // Forward most methods to the wrapped tower by default
    void upgrade() override { wrappedTower->upgrade(); }
    int getRefundValue() const override { return wrappedTower->getRefundValue(); }
    void draw() const override { wrappedTower->draw(); }
    
    float getRange() const override { return wrappedTower->getRange(); }
    float getPower() const override { return wrappedTower->getPower(); }
    float getFireRate() const override { return wrappedTower->getFireRate(); }
    int getBuyCost() const override { return wrappedTower->getBuyCost(); }
    int getUpgradeCost() const override { return wrappedTower->getUpgradeCost(); }
    int getLevel() const override { return wrappedTower->getLevel(); }
    const std::string& getName() const override { return wrappedTower->getName(); }
    Vector2 getPosition() const override { return wrappedTower->getPosition(); }
    
    void setPosition(Vector2 pos) override { wrappedTower->setPosition(pos); }
    bool canShoot() const override { return wrappedTower->canShoot(); }
    void resetShotTimer() override { wrappedTower->resetShotTimer(); }
    
    void setTargetingStrategy(std::unique_ptr<ITargetingStrategy> strategy) override {
        wrappedTower->setTargetingStrategy(std::move(strategy));
    }
    
    // This is a decorator-specific method - gets through the wrappers to the core tower
    virtual ITower* getBaseTower() {
        if (auto* decorator = dynamic_cast<TowerDecorator*>(wrappedTower.get())) {
            return decorator->getBaseTower();
        }
        return wrappedTower.get();
    }
    
    // Override the attackCritters method as it's the core behavior decorators will modify
    void attackCritters(std::vector<Critter>& critters) override {
        wrappedTower->attackCritters(critters);
    }
};

// Concrete Decorators

// Splash Damage Decorator - Applies damage to nearby critters
class SplashDecorator : public TowerDecorator {
private:
    float splashRadius;
    float splashDamagePercent;
    
public:
    SplashDecorator(std::unique_ptr<ITower> tower, float radius = 50.0f, float damagePercent = 0.5f)
        : TowerDecorator(std::move(tower)), splashRadius(radius), splashDamagePercent(damagePercent) {}
    
    void attackCritters(std::vector<Critter>& critters) override;
    
    void draw() const override {
        wrappedTower->draw();
        // Add a splash indicator to the tower
        DrawCircleLines(getPosition().x, getPosition().y, 18, ORANGE);
    }
};

// Burning Effect Decorator - Applies damage over time
class BurningDecorator : public TowerDecorator {
private:
    float burnDuration;
    float burnDamagePerSecond;
    std::vector<std::pair<Critter*, float>> burningCritters; // Critter and remaining burn time
    
public:
    BurningDecorator(std::unique_ptr<ITower> tower, float duration = 3.0f, float dps = 5.0f)
        : TowerDecorator(std::move(tower)), burnDuration(duration), burnDamagePerSecond(dps) {}
    
    void attackCritters(std::vector<Critter>& critters) override;
    
    void draw() const override {
        wrappedTower->draw();
        // Add a fire indicator to the tower
        DrawCircleLines(getPosition().x, getPosition().y, 18, RED);
    }
};

// Freezing Effect Decorator - Slows down critters
class FreezingDecorator : public TowerDecorator {
private:
    float freezeEffect;  // Slow percentage (0-1)
    float freezeDuration;  // Duration of freeze effect
    
public:
    FreezingDecorator(std::unique_ptr<ITower> tower, float effect = 0.5f, float duration = 2.0f)
        : TowerDecorator(std::move(tower)), freezeEffect(effect), freezeDuration(duration) {}
    
    void attackCritters(std::vector<Critter>& critters) override;
    
    void draw() const override {
        wrappedTower->draw();
        // Add a freeze indicator to the tower
        DrawCircleLines(getPosition().x, getPosition().y, 18, SKYBLUE);
    }
};
