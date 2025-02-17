/**
 * @file critter.cpp
 * @brief Implementation of the Critter class for the Tower Defense game.
 */

#include "critter.h"

/**
 * @brief Constructs a Critter with given attributes and initial position.
 *
 * @param hp Initial hit points of the critter.
 * @param str Strength of the critter, affecting the amount of coins stolen upon exit.
 * @param spd Movement speed of the critter.
 * @param lvl Level of the critter, used for scaling difficulty.
 * @param rwd Reward value given to the player upon critter death.
 * @param pos Initial position of the critter on the map.
 * @param gameMap Pointer to the game map to determine movement.
 */
Critter::Critter(int hp, int str, int spd, int lvl, int rwd, pair<int, int> pos, const Map* gameMap) {
    hitPoints = hp;
    strength = str;
    speed = spd;
    level = lvl;
    reward = rwd;
    position = pos;
    reachedExit = false;
    map = gameMap;
}

/**
 * @brief Moves the critter towards the exit point based on its speed.
 *
 * The critter moves one step at a time towards the exit point. If it reaches
 * the exit, it sets the `reachedExit` flag to true. Movement is restricted
 * to path cells as defined by the map.
 */
void Critter::move() {
    if (reachedExit || isDead()) {
        return;
    }

    pair<int, int> exitPoint = map->getExit();

    // Move towards exit point
    for (int moves = 0; moves < speed; moves++) {
        // Check if we are at the exit
        if(position.first == exitPoint.first && position.second == exitPoint.second) {
            reachedExit = true;
            return;
        }

        // Determine next valid move towards exit
        pair<int, int> nextPosition = position;
        if (position.first < exitPoint.first && map->isPath(position.first + 1, position.second)) {
            nextPosition.first++; // move right
        }
        else if (position.second < exitPoint.second && map->isPath(position.first, position.second + 1)) {
            nextPosition.second++; // move down
        }
        else if (position.second > exitPoint.second && map->isPath(position.first, position.second - 1)) {
            nextPosition.second--; // move up
        }
        else {
            break; // No valid moves available
        }

        // Update position
        position = nextPosition;
    }
}

/**
 * @brief Reduces the critter's hit points by a given damage amount.
 *
 * @param damage Amount of hit points to subtract.
 */
void Critter::takeDamage(int damage) {
    hitPoints -= damage;
}

/**
 * @brief Checks if the critter is dead (hit points <= 0).
 *
 * @return True if the critter has zero or less hit points, false otherwise.
 */
bool Critter::isDead() const {
    return hitPoints <= 0;
}

// Getter and setter methods
/** @brief Gets the current hit points of the critter. */
int Critter::getHitPoints() const { return hitPoints; }

/** @brief Gets the strength of the critter. */
int Critter::getStrength() const { return strength; }

/** @brief Gets the movement speed of the critter. */
int Critter::getSpeed() const { return speed; }

/** @brief Gets the level of the critter. */
int Critter::getLevel() const { return level; }

/** @brief Gets the reward value of the critter. */
int Critter::getReward() const { return reward; }

/** @brief Checks if the critter has reached the exit point. */
bool Critter::hasReachedExit() const { return reachedExit; }

/** @brief Gets the current position of the critter on the map. */
pair<int, int> Critter::getPosition() const { return position; }

/**
 * @brief Sets the position of the critter on the map.
 *
 * @param x X-coordinate of the new position.
 * @param y Y-coordinate of the new position.
 */
void Critter::setPosition(int x, int y) {
    position = make_pair(x, y);
}

/**
 * @brief Sets the movement speed of the critter.
 *
 * @param spd New speed value.
 */
void Critter::setSpeed(int spd) {
    speed = spd;
}
