/**
 * @file tower.h
 * @brief Declaration of the Tower class for the Tower Defense game.
 */

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
 * - Attack power, range, fire rate
 * - Purchase & refund value
 * - Upgrade mechanics
 */
class Tower {
protected:
    int x, y;        ///< Tower position on the map
    int buyCost;     ///< Gold cost to purchase the tower
    int refundValue; ///< Gold refunded when selling the tower
    int range;       ///< Attack range (radius in grid units)
    int power;       ///< Attack damage
    int fireRate;    ///< Attack speed (shots per second)
    int level;       ///< Tower level (1-3)
    int upgradeCost; ///< Gold required to upgrade

public:
    Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost);
    virtual ~Tower() {}

    virtual void attack(vector<Critter>& critters) = 0;
    void upgrade();

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
 */
class BasicTower : public Tower {
public:
    BasicTower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

/**
 * @class AoETower
 * @brief A tower that deals area-of-effect (AoE) damage.
 */
class AoETower : public Tower {
public:
    AoETower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

/**
 * @brief Allows the user to place a tower interactively.
 * @param map Reference to the game map.
 * @param towers Vector of currently placed towers.
 */
void placeTowerInteractive(Map& map, vector<Tower*>& towers);

#endif // TOWER_H
