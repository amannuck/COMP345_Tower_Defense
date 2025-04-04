/**
 * @file Tower.h
 * @brief Defines tower classes using the Decorator pattern
 * @details This file provides interfaces and implementations for various tower types
 *          in the tower defense game. The design uses the Decorator pattern to allow
 *          dynamic enhancement of tower capabilities.
 */

#pragma once
#include <raylib.h>
#include <string>
#include <memory>

#include "Critter.h"
#include "TargetingStrategy.h"

/**
 * @class ITower
 * @brief Abstract interface for all tower types
 * @details Defines the common interface that all towers must implement,
 *          including basic functionality and property access.
 */
class ITower {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~ITower() = default;
    
    /**
     * @brief Upgrades the tower, improving its capabilities
     */
    virtual void upgrade() = 0;
    
    /**
     * @brief Attacks critters within range
     * @param critters Vector of critters to potentially target
     */
    virtual void attackCritters(std::vector<Critter>& critters) = 0;
    
    /**
     * @brief Calculates the refund value for selling the tower
     * @return Currency amount refunded when selling
     */
    virtual int getRefundValue() const = 0;
    
    /**
     * @brief Renders the tower on the screen
     */
    virtual void draw() const = 0;
    
    /**
     * @brief Gets the tower's attack range
     * @return Attack range in pixels
     */
    virtual float getRange() const = 0;
    
    /**
     * @brief Gets the tower's attack power
     * @return Damage dealt per attack
     */
    virtual float getPower() const = 0;
    
    /**
     * @brief Gets the tower's fire rate
     * @return Attacks per second
     */
    virtual float getFireRate() const = 0;
    
    /**
     * @brief Gets the tower's purchase cost
     * @return Initial cost in currency
     */
    virtual int getBuyCost() const = 0;
    
    /**
     * @brief Gets the tower's upgrade cost
     * @return Cost to upgrade in currency
     */
    virtual int getUpgradeCost() const = 0;
    
    /**
     * @brief Gets the tower's current level
     * @return Current upgrade level
     */
    virtual int getLevel() const = 0;
    
    /**
     * @brief Gets the tower's name
     * @return Reference to the tower's name
     */
    virtual const std::string& getName() const = 0;
    
    /**
     * @brief Gets the tower's position
     * @return 2D position vector
     */
    virtual Vector2 getPosition() const = 0;
    
    /**
     * @brief Sets the tower's position
     * @param pos New position vector
     */
    virtual void setPosition(Vector2 pos) = 0;
    
    /**
     * @brief Checks if the tower can attack
     * @return true if the tower is ready to attack, false otherwise
     */
    virtual bool canShoot() const = 0;
    
    /**
     * @brief Resets the tower's attack timer
     */
    virtual void resetShotTimer() = 0;
    
    /**
     * @brief Sets the tower's targeting strategy
     * @param strategy Unique pointer to targeting strategy
     */
    virtual void setTargetingStrategy(std::unique_ptr<ITargetingStrategy> strategy) = 0;
};

/**
 * @class Tower
 * @brief Concrete base implementation of the ITower interface
 * @details Provides the core functionality for all tower types.
 */
class Tower : public ITower {
protected:
    int level;                                    ///< Current tower level
    float range;                                  ///< Attack range in pixels
    float power;                                  ///< Damage dealt per attack
    float fireRate;                               ///< Attacks per second
    float lastShotTime;                           ///< Time of last attack
    Vector2 position;                             ///< Tower position
    int buyCost;                                  ///< Initial purchase cost
    int upgradeCost;                              ///< Cost to upgrade
    std::string name;                             ///< Display name
    Color color;                                  ///< Tower color for rendering
    std::unique_ptr<ITargetingStrategy> targetingStrategy; ///< Strategy for target selection

public:
    /**
     * @brief Constructor for Tower
     * @param range Maximum attack range
     * @param power Damage per attack
     * @param fireRate Attacks per second
     * @param buyCost Initial purchase cost
     * @param upgradeCost Cost to upgrade
     * @param name Display name
     * @param color Tower color
     */
    Tower(float range, float power, float fireRate, int buyCost, int upgradeCost, 
          const std::string& name, const Color& color);
    
    /**
     * @brief Destructor for Tower
     */
    virtual ~Tower();

    /**
     * @brief Upgrades the tower's capabilities
     * @details Increases level, range, power, fire rate, and upgrade cost
     */
    void upgrade() override;
    
    /**
     * @brief Attacks critters within range
     * @param critters Vector of critters to potentially target
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Calculates refund value for selling
     * @return Currency amount refunded when selling
     */
    int getRefundValue() const override;
    
    /**
     * @brief Renders the tower
     */
    void draw() const override;
    
    /**
     * @brief Sets the targeting strategy
     * @param strategy Unique pointer to targeting strategy
     */
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

/**
 * @brief Updates all active tower shots
 * @param deltaTime Time elapsed since last update
 */
void UpdateTowerShots(float deltaTime);

/**
 * @brief Renders all active tower shots
 */
void DrawTowerShots();

/**
 * @struct TowerShot
 * @brief Represents a projectile fired by a tower
 */
struct TowerShot {
    Vector2 start;      ///< Starting position
    Vector2 end;        ///< Target position
    Vector2 currentPos; ///< Current position during animation
    float speed;        ///< Movement speed in pixels per second
    Color color;        ///< Projectile color
    float timer;        ///< Remaining lifetime
    bool hit;           ///< Whether the projectile has hit its target
};

/**
 * @class BasicTower
 * @brief Standard balanced tower implementation
 * @details Provides a balanced tower with moderate range, power, and fire rate.
 */
class BasicTower : public Tower {
public:
    /**
     * @brief Constructor for BasicTower
     */
    BasicTower();
    
    /**
     * @brief Upgrades the BasicTower
     * @details Adds additional power boost on upgrade
     */
    void upgrade() override;
};

/**
 * @class AreaTower
 * @brief Tower that damages multiple targets in an area
 * @details Specializes in attacking groups of critters with area effect damage.
 */
class AreaTower : public Tower {
private:
    float areaRadius; ///< Radius of area effect
    
public:
    /**
     * @brief Constructor for AreaTower
     */
    AreaTower();
    
    /**
     * @brief Upgrades the AreaTower
     * @details Increases area radius on upgrade
     */
    void upgrade() override;
    
    /**
     * @brief Area attack implementation
     * @param critters Vector of critters to potentially target
     * @details Damages all critters within area radius of the primary target
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Gets the area effect radius
     * @return Area radius in pixels
     */
    float getAreaRadius() const { return areaRadius; }
};

/**
 * @class SlowTower
 * @brief Tower that reduces critter movement speed
 * @details Specializes in slowing down critters to delay their progress.
 */
class SlowTower : public Tower {
private:
    float slowEffect;    ///< Percentage to slow enemies (0-1)
    float slowDuration;  ///< Duration of slow effect in seconds
    
public:
    /**
     * @brief Constructor for SlowTower
     */
    SlowTower();
    
    /**
     * @brief Upgrades the SlowTower
     * @details Increases slow effect and duration on upgrade
     */
    void upgrade() override;
    
    /**
     * @brief Slow attack implementation
     * @param critters Vector of critters to potentially target
     * @details Damages and applies movement speed reduction to target
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Gets the slow effect percentage
     * @return Slow effect as a decimal (0-1)
     */
    float getSlowEffect() const { return slowEffect; }
    
    /**
     * @brief Gets the slow effect duration
     * @return Duration in seconds
     */
    float getSlowDuration() const { return slowDuration; }
};

/**
 * @class SniperTower
 * @brief Long-range tower with critical hit capability
 * @details Specializes in high damage to single targets at long range.
 */
class SniperTower : public Tower {
private:
    float criticalChance;     ///< Chance for critical hit (0-1)
    float criticalMultiplier; ///< Damage multiplier for critical hits
    
public:
    /**
     * @brief Constructor for SniperTower
     */
    SniperTower();
    
    /**
     * @brief Upgrades the SniperTower
     * @details Increases range and critical chance on upgrade
     */
    void upgrade() override;
    
    /**
     * @brief Sniper attack implementation
     * @param critters Vector of critters to potentially target
     * @details Deals high damage with chance for critical hits
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Gets the critical hit chance
     * @return Chance as a decimal (0-1)
     */
    float getCriticalChance() const { return criticalChance; }
    
    /**
     * @brief Gets the critical hit damage multiplier
     * @return Multiplier for critical hit damage
     */
    float getCriticalMultiplier() const { return criticalMultiplier; }
};

/**
 * @class TowerDecorator
 * @brief Base decorator for adding functionality to towers
 * @details Implements the Decorator pattern to allow dynamic enhancement
 *          of tower capabilities without modifying the original classes.
 */
class TowerDecorator : public ITower {
protected:
    std::unique_ptr<ITower> wrappedTower; ///< Tower being decorated
    
public:
    /**
     * @brief Constructor for TowerDecorator
     * @param tower Unique pointer to tower being decorated
     */
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
    
    /**
     * @brief Gets the original tower through all decorators
     * @return Pointer to the base tower
     * @details Recursively unwraps decorators to find the core tower
     */
    virtual ITower* getBaseTower() {
        if (auto* decorator = dynamic_cast<TowerDecorator*>(wrappedTower.get())) {
            return decorator->getBaseTower();
        }
        return wrappedTower.get();
    }
    
    /**
     * @brief Decorator-specific attack implementation
     * @param critters Vector of critters to potentially target
     * @details Default implementation forwards to wrapped tower
     */
    void attackCritters(std::vector<Critter>& critters) override {
        wrappedTower->attackCritters(critters);
    }
};

/**
 * @class SplashDecorator
 * @brief Decorator that adds splash damage to towers
 * @details Enhances a tower by causing its attacks to damage multiple
 *          critters in a radius around the primary target.
 */
class SplashDecorator : public TowerDecorator {
private:
    float splashRadius;       ///< Radius of splash effect
    float splashDamagePercent; ///< Percentage of main damage applied to nearby critters
    
public:
    /**
     * @brief Constructor for SplashDecorator
     * @param tower Tower to decorate
     * @param radius Splash damage radius
     * @param damagePercent Percentage of main damage applied to splash targets
     */
    SplashDecorator(std::unique_ptr<ITower> tower, float radius = 50.0f, float damagePercent = 0.5f)
        : TowerDecorator(std::move(tower)), splashRadius(radius), splashDamagePercent(damagePercent) {}
    
    /**
     * @brief Enhanced attack with splash damage
     * @param critters Vector of critters to potentially target
     * @details Damages the primary target and nearby critters
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Renders the tower with splash indicator
     */
    void draw() const override {
        wrappedTower->draw();
        // Add a splash indicator to the tower
        DrawCircleLines(getPosition().x, getPosition().y, 18, ORANGE);
    }
};

/**
 * @class BurningDecorator
 * @brief Decorator that adds damage over time to towers
 * @details Enhances a tower by causing its attacks to apply a burning effect
 *          that continues to damage critters over time.
 */
class BurningDecorator : public TowerDecorator {
private:
    float burnDuration;           ///< Duration of burning effect in seconds
    float burnDamagePerSecond;    ///< Damage per second from burning
    std::vector<std::pair<Critter*, float>> burningCritters; ///< Critters with active burning effect and remaining time
    
public:
    /**
     * @brief Constructor for BurningDecorator
     * @param tower Tower to decorate
     * @param duration Duration of burning effect
     * @param dps Damage per second from burning
     */
    BurningDecorator(std::unique_ptr<ITower> tower, float duration = 3.0f, float dps = 5.0f)
        : TowerDecorator(std::move(tower)), burnDuration(duration), burnDamagePerSecond(dps) {}
    
    /**
     * @brief Enhanced attack with burning effect
     * @param critters Vector of critters to potentially target
     * @details Damages the target and applies burning effect
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Renders the tower with burning indicator
     */
    void draw() const override {
        wrappedTower->draw();
        // Add a fire indicator to the tower
        DrawCircleLines(getPosition().x, getPosition().y, 18, RED);
    }
};

/**
 * @class FreezingDecorator
 * @brief Decorator that adds slowing effect to towers
 * @details Enhances a tower by causing its attacks to slow down
 *          the movement speed of targeted critters.
 */
class FreezingDecorator : public TowerDecorator {
private:
    float freezeEffect;   ///< Slow percentage (0-1)
    float freezeDuration; ///< Duration of freeze effect in seconds
    
public:
    /**
     * @brief Constructor for FreezingDecorator
     * @param tower Tower to decorate
     * @param effect Slow percentage as decimal (0-1)
     * @param duration Duration of freezing effect
     */
    FreezingDecorator(std::unique_ptr<ITower> tower, float effect = 0.5f, float duration = 2.0f)
        : TowerDecorator(std::move(tower)), freezeEffect(effect), freezeDuration(duration) {}
    
    /**
     * @brief Enhanced attack with freezing effect
     * @param critters Vector of critters to potentially target
     * @details Damages the target and applies movement speed reduction
     */
    void attackCritters(std::vector<Critter>& critters) override;
    
    /**
     * @brief Renders the tower with freezing indicator
     */
    void draw() const override {
        wrappedTower->draw();
        // Add a freeze indicator to the tower
        DrawCircleLines(getPosition().x, getPosition().y, 18, SKYBLUE);
    }
};
