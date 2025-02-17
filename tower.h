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
 *
 * Towers can now be placed and removed from the map.
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
    /**
     * @brief Constructs a Tower object.
     * @param x X-coordinate of the tower.
     * @param y Y-coordinate of the tower.
     * @param cost Initial cost of the tower.
     * @param refund Refund value when selling the tower.
     * @param range Attack range of the tower.
     * @param power Attack power of the tower.
     * @param fireRate Rate of fire of the tower.
     * @param upgradeCost Cost to upgrade the tower.
     */
    Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost);
    virtual ~Tower() {}

    /**
     * @brief Attacks critters within range.
     * @param critters List of critters on the map.
     */
    virtual void attack(vector<Critter>& critters) = 0;

    /**
     * @brief Upgrades the tower (increases damage and refund value).
     */
    void upgrade();

    /**
     * @brief Gets the X-coordinate of the tower.
     * @return X-coordinate.
     */
    int getX() { return x; }

    /**
     * @brief Gets the Y-coordinate of the tower.
     * @return Y-coordinate.
     */
    int getY() { return y; }

    /**
     * @brief Gets the attack range of the tower.
     * @return Attack range.
     */
    int getRange() { return range; }

    /**
     * @brief Gets the attack power of the tower.
     * @return Attack power.
     */
    int getPower() { return power; }

    /**
     * @brief Gets the purchase cost of the tower.
     * @return Purchase cost.
     */
    int getBuyCost() { return buyCost; }

    /**
     * @brief Gets the refund value of the tower.
     * @return Refund value.
     */
    int getRefundValue() { return refundValue; }

    /**
     * @brief Gets the current level of the tower.
     * @return Tower level.
     */
    int getLevel() { return level; }
};

/**
 * @class BasicTower
 * @brief A tower that deals single-target damage.
 */
class BasicTower : public Tower {
public:
    /**
     * @brief Constructs a BasicTower object.
     * @param x X-coordinate of the tower.
     * @param y Y-coordinate of the tower.
     */
    BasicTower(int x, int y);

    /**
     * @brief Attacks a single critter within range.
     * @param critters List of critters on the map.
     */
    void attack(vector<Critter>& critters) override;
};

/**
 * @class AoETower
 * @brief A tower that deals area-of-effect (AoE) damage.
 */
class AoETower : public Tower {
public:
    /**
     * @brief Constructs an AoETower object.
     * @param x X-coordinate of the tower.
     * @param y Y-coordinate of the tower.
     */
    AoETower(int x, int y);

    /**
     * @brief Attacks multiple critters within range.
     * @param critters List of critters on the map.
     */
    void attack(vector<Critter>& critters) override;
};


#endif // TOWER_H