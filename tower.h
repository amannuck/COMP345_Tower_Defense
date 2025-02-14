#ifndef TOWER_H
#define TOWER_H

#include <iostream>
#include <vector>
#include "mapgen.h"
#include "critter.h"

using namespace std;

/**
 * @class Tower
 * @brief Base class for all tower types.
 *
 * This class defines the fundamental properties of a tower, including:
 * - Position (x, y)
 * - Attack power
 * - Attack range
 * - Purchase & refund value
 * - Upgrade mechanics
 * 
 * Different tower types (single-target, AoE, slow) inherit from this class.
 */
class Tower {
protected:
    int x, y;        // Tower position on the map
    int buyCost;     // Gold cost to purchase the tower
    int refundValue; // Gold refunded when selling the tower
    int range;       // Attack range (radius in grid units)
    int power;       // Attack damage
    int fireRate;    // Attack speed (shots per second)
    int level;       // Tower level (1-3)
    int upgradeCost; // Gold required to upgrade the tower

public:
    Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost);
    virtual ~Tower() {}

    // Attack method (to be implemented in subclasses)
    virtual void attack(vector<Critter>& critters) = 0;

    // Upgrades the tower (increases damage and refund value)
    void upgrade();

    // Getter methods
    int getX() { return x; }
    int getY() { return y; }
    int getRange() { return range; }
    int getPower() { return power; }
    int getBuyCost() { return buyCost; }
    int getRefundValue() { return refundValue; }
    int getLevel() { return level; }
};

/**
 * @class BasicTower
 * @brief A tower that deals single-target damage.
 *
 * This tower attacks one enemy at a time, making it ideal for high-health enemies.
 */
class BasicTower : public Tower {
public:
    BasicTower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

/**
 * @class AoETower
 * @brief A tower that deals area-of-effect (AoE) damage.
 *
 * This tower attacks multiple enemies within its range at once, but its damage is lower.
 */
class AoETower : public Tower {
public:
    AoETower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

/**
 * @class SlowTower
 * @brief A tower that slows down enemies.
 *
 * This tower does not deal damage but slows enemies within its range, making them easier targets.
 */
class SlowTower : public Tower {
public:
    SlowTower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

/**
 * @class TowerManager
 * @brief Manages towers (purchasing, selling, upgrading).
 *
 * The TowerManager is responsible for:
 * - Purchasing towers and placing them on the map
 * - Upgrading existing towers
 * - Selling towers and refunding gold
 * - Managing tower attacks on critters
 */
class TowerManager {
private:
    vector<Tower*> towers; // List of all towers
    int playerGold;        // Player's available gold
    Map* map;              // Pointer to the game map

public:
    TowerManager(Map* gameMap, int initialGold);
    ~TowerManager();

    bool buyTower(int x, int y, string type);
    bool sellTower(int x, int y);
    bool upgradeTower(int x, int y);
    void updateTowers(vector<Critter>& critters);
    void displayTowers();
};

#endif // TOWER_H
