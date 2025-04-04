/**
 * @file Critter.cpp
 * @brief Implementation of the Critter class for enemies in the tower defense game
 * @details This file provides the implementation for critter movement, health management,
 *          status effects, and the observer pattern for critter events.
 */

#include "Critter.h"
#include <cmath>
#include <iostream>
#include <algorithm>  // For std::remove and std::find

#include "raymath.h"

/**
 * @brief Constructor for the Critter class
 * @param level Current game level affecting critter stats
 * @param speed Movement speed in pixels per second
 * @param hp Initial and maximum hit points
 * @param reward Currency awarded when defeated
 * @param strength Damage to player when reaching the end
 * @param path Vector of waypoints for the critter to follow
 * @param type String identifier for the critter type
 */
Critter::Critter(int level, float speed, float hp, int reward, int strength,
                 const std::vector<Vector2>& path, const std::string& type)
    : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp),
      reward(reward), strength(strength), position(path.front()),
      path(path), pathIndex(0), active(false), type(type) {}

/**
 * @brief Applies a movement speed reduction effect to the critter
 * @param factor Multiplier for the critter's speed (0-1)
 * @param duration Time in seconds the effect should last
 * @details Only applies the effect if it's stronger or has a longer duration
 *          than any existing slow effect on the critter.
 */
void Critter::applySlowEffect(float factor, float duration) {
    if (factor < slowFactor || (factor == slowFactor && duration > slowDuration)) {
        slowFactor = factor;
        slowDuration = duration;
    }
}

/**
 * @brief Updates the critter's position along its path
 * @details Handles movement between waypoints, applies slow effects,
 *          and notifies observers when the critter reaches the end.
 */
void Critter::move() {
    if (slowDuration > 0) {
        slowDuration -= GetFrameTime();
        if (slowDuration <= 0) {
            slowFactor = 1.0f;
        }
    }

    if (pathIndex < path.size() - 1) {
        Vector2 target = path[pathIndex + 1];
        Vector2 direction = Vector2Subtract(target, position);
        direction = Vector2Normalize(direction);

        float effectiveSpeed = speed * slowFactor;
        position = Vector2Add(position, Vector2Scale(direction, effectiveSpeed * GetFrameTime()));

        if (Vector2Distance(position, target) < 1.0f) {
            pathIndex++;
            if (pathIndex >= path.size() - 1) {
                reachedEndFlag = true;
                active = false;
                notifyReachedEnd();
            }
        }
    } else if (!reachedEndFlag) {
        reachedEndFlag = true;
        active = false;
        notifyReachedEnd();
    }
}

/**
 * @brief Reduces the critter's hit points and handles defeat
 * @param damage Amount of hit points to deduct
 * @details Notifies observers if the damage reduces health to zero
 */
void Critter::takeDamage(float damage) {
    hitPoints -= damage;
    if (isDead()) {
        notifyDefeated();
    }
}

/**
 * @brief Checks if the critter has been defeated
 * @return true if the critter's hit points are zero or less
 */
bool Critter::isDead() const {
    return hitPoints <= 0;
}

/**
 * @brief Checks if the critter has reached the end of its path
 * @return true if the critter reached the final waypoint
 */
bool Critter::reachedEnd() const {
    return reachedEndFlag || pathIndex >= path.size() - 1;
}

/**
 * @brief Renders the critter on screen
 * @details Draws the critter with different colors based on type,
 *          includes a health bar and visual indicators for status effects.
 */
void Critter::draw() const {
    if (hitPoints <= 0) return;

    // Different colors for different critter types
    Color critterColor;
    if (type == "Basic") critterColor = RED;
    else if (type == "Fast") critterColor = GREEN;
    else if (type == "Tank") critterColor = GRAY;
    else if (type == "Boss") critterColor = PURPLE;
    else critterColor = ORANGE;  // Default color for unknown types

    DrawCircleV(position, 10, critterColor);

    // Health bar
    float healthBarWidth = 20 * ((float)hitPoints / (float)maxHitPoints);
    DrawRectangle(position.x - 10, position.y - 15, healthBarWidth, 5, GREEN);

    // Slow effect indicator
    if (slowFactor < 1.0f) {
        DrawCircleV(position, 13, ColorAlpha(BLUE, 0.5f));
        DrawText("SLOW", position.x - 15, position.y - 25, 10, BLUE);
    }
}

/**
 * @brief Activates the critter, allowing it to start moving
 */
void Critter::activate() {
    active = true;
}

/**
 * @brief Registers an observer to receive critter event notifications
 * @param observer Pointer to the observer to register
 * @details Checks for null observers and prevents duplicate registrations
 */
void Critter::addObserver(CritterObserver* observer) {
    if (!observer) {
        std::cerr << "ERROR: Attempted to add a null observer." << std::endl;
        return;
    }

    if (std::find(observers.begin(), observers.end(), observer) == observers.end()) {
        observers.push_back(observer);
        std::cout << "Observer added successfully." << std::endl;
    } else {
        std::cerr << "WARNING: Observer already exists in the list." << std::endl;
    }
}

/**
 * @brief Unregisters an observer from receiving notifications
 * @param observer Pointer to the observer to unregister
 * @details Checks for null observers and whether the observer exists in the list
 */
void Critter::removeObserver(CritterObserver* observer) {
    if (!observer) {
        std::cerr << "ERROR: Attempted to remove a null observer." << std::endl;
        return;
    }

    auto it = std::remove(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it, observers.end());
        std::cout << "Observer removed successfully." << std::endl;
    } else {
        std::cerr << "WARNING: Observer not found in the list." << std::endl;
    }
}

/**
 * @brief Notifies all observers that the critter has reached the end of its path
 * @details Calls onCritterReachedEnd on all registered observers
 */
void Critter::notifyReachedEnd() {
    std::cout << "Notifying " << observers.size() << " observers that critter reached end" << std::endl;
    for (auto observer : observers) {
        if (observer) {
            std::cout << "Calling onCritterReachedEnd for observer" << std::endl;
            observer->onCritterReachedEnd(*this);
        } else {
            std::cerr << "ERROR: Null observer in the list!" << std::endl;
        }
    }
}

/**
 * @brief Notifies all observers that the critter has been defeated
 * @details Calls onCritterDefeated on all registered observers
 */
void Critter::notifyDefeated() {
    for (auto observer : observers) {
        if (observer) {
            observer->onCritterDefeated(*this);
        }
    }
}
