/**
 * @file critter.h
 * @brief Declaration of the Critter class for the Tower Defense game.
 */

#ifndef CRITTER_H
#define CRITTER_H

#include <utility>
#include "mapgen.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * @class Critter
 * @brief Represents a single enemy unit in the tower defense game.
 *
 * Critters move from the entry point of the map towards the exit, following a predefined path.
 * They can be attacked by towers, reducing their health until they either reach the exit or die.
 */
class Critter {
private:
    int hitPoints;       ///< Current health of the critter
    int strength;        ///< Damage dealt to player when reaching exit
    int speed;           ///< Movement speed of the critter
    int level;           ///< Level of the critter
    int reward;          ///< Coins awarded when killed
    bool reachedExit;    ///< Flag to track if critter reached exit
    pair<int, int> position;  ///< Current position on the map
    const Map* map;      ///< Pointer to the map object

public:
    /**
     * @brief Constructs a new Critter object with given attributes.
     *
     * @param hp Initial hit points.
     * @param str Strength (damage to player if reaching exit).
     * @param spd Movement speed.
     * @param lvl Critter level.
     * @param rwd Coin reward when killed.
     * @param pos Initial position on the map.
     * @param gameMap Pointer to the game map.
     */
    Critter(int hp, int str, int spd, int lvl, int rwd, pair<int, int> pos, const Map* gameMap);

    /**
     * @brief Moves the critter towards the exit point based on its speed.
     */
    void move();

    /**
     * @brief Reduces the critter's hit points by a given damage amount.
     *
     * @param damage Amount of hit points to subtract.
     */
    void takeDamage(int damage);

    /**
     * @brief Checks if the critter is dead (hit points <= 0).
     *
     * @return True if the critter has zero or less hit points, false otherwise.
     */
    bool isDead() const;

    // Getter and setter methods
    /** @brief Gets the current hit points of the critter. */
    int getHitPoints() const;

    /** @brief Gets the strength of the critter. */
    int getStrength() const;

    /** @brief Gets the movement speed of the critter. */
    int getSpeed() const;

    /** @brief Gets the level of the critter. */
    int getLevel() const;

    /** @brief Gets the reward value of the critter. */
    int getReward() const;

    /** @brief Checks if the critter has reached the exit point. */
    bool hasReachedExit() const;

    /** @brief Gets the current position of the critter on the map. */
    pair<int, int> getPosition() const;

    /**
     * @brief Sets the movement speed of the critter.
     *
     * @param spd New speed value.
     */
    void setSpeed(int spd);

    /**
     * @brief Sets the position of the critter on the map.
     *
     * @param x X-coordinate of the new position.
     * @param y Y-coordinate of the new position.
     */
    void setPosition(int x, int y);
};

#endif // CRITTER_H
