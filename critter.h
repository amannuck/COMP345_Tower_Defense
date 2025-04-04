/**
 * @file Critter.h
 * @brief Defines the Critter class for enemies in the tower defense game
 * @details This class represents the enemies that travel along paths and are
 *          targeted by towers. It includes movement, health, status effects,
 *          and observer pattern implementation.
 */

#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "CritterObserver.h"

/**
 * @class Critter
 * @brief Represents an enemy unit in the tower defense game
 * @details Handles movement along paths, health management, status effects,
 *          and notifications to observers when important events occur.
 */
class Critter {
private:
    int level;                    ///< Current level affecting critter stats
    float speed;                  ///< Movement speed in pixels per second
    float hitPoints;              ///< Current health points
    float maxHitPoints;           ///< Maximum health points
    int reward;                   ///< Currency given to player when defeated
    int strength;                 ///< Damage to player when reaching the end
    Vector2 position;             ///< Current position on screen
    std::vector<Vector2> path;    ///< Waypoints to follow
    int pathIndex;                ///< Current position in the path sequence
    bool active;                  ///< Whether the critter is currently active
    bool reachedEndFlag = false;  ///< Flag indicating if reached the end
    std::string type;             ///< Type of critter (Basic, Fast, Tank, Boss)
    
    // Slow effect variables
    float slowFactor = 1.0f;      ///< Movement speed multiplier (0-1)
    float slowDuration = 0.0f;    ///< Remaining time for slow effect
    
    std::vector<CritterObserver*> observers; ///< Collection of registered observers

public:
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
    Critter(int level, float speed, float hp, int reward, int strength, 
            const std::vector<Vector2>& path, const std::string& type);
    
    /**
     * @brief Sets the critter's position based on grid coordinates
     * @param gridPos Grid position coordinates
     * @param cellSize Size of each grid cell
     * @param offsetX X-offset for the grid
     * @param offsetY Y-offset for the grid
     */
    void setPosition(const Vector2 &gridPos, int cellSize, int offsetX, int offsetY);
    
    /**
     * @brief Updates the critter's position along its path
     * @details Handles movement between waypoints, applies slow effects,
     *          and notifies observers when the critter reaches the end.
     */
    void move();
    
    /**
     * @brief Reduces the critter's hit points and handles defeat
     * @param damage Amount of hit points to deduct
     */
    void takeDamage(float damage);
    
    /**
     * @brief Checks if the critter has been defeated
     * @return true if the critter's hit points are zero or less
     */
    bool isDead() const;
    
    /**
     * @brief Checks if the critter has reached the end of its path
     * @return true if the critter reached the final waypoint
     */
    bool reachedEnd() const;
    
    /**
     * @brief Renders the critter on screen
     */
    void draw() const;
    
    /**
     * @brief Activates the critter, allowing it to start moving
     */
    void activate();
    
    /**
     * @brief Checks if the critter is currently active
     * @return true if the critter is active, false otherwise
     */
    bool isActive() const { return active; }
    
    /**
     * @brief Sets the critter's position directly
     * @param newPos New position vector
     */
    void setPosition(const Vector2& newPos) { position = newPos; }
    
    /**
     * @brief Gets the critter's current position
     * @return Position vector
     */
    const Vector2 getPosition() const { return position; }
    
    /**
     * @brief Sets the critter's active state
     * @param state New active state
     */
    void setActive(bool state) { active = state; }
    
    /**
     * @brief Gets the reward value for defeating this critter
     * @return Reward amount in currency
     */
    int getReward() const { return reward; }
    
    /**
     * @brief Gets the strength value (damage to player)
     * @return Strength value
     */
    int getStrength() const { return strength; }
    
    /**
     * @brief Gets the current hit points
     * @return Current hit points
     */
    float getHitPoints() const { return hitPoints; }
    
    /**
     * @brief Gets the current path index
     * @return Index in the path sequence
     */
    int getPathIndex() const { return pathIndex; }
    
    /**
     * @brief Gets the critter's type
     * @return String identifier for the critter type
     */
    const std::string& getType() const { return type; }
    
    /**
     * @brief Applies a movement speed reduction effect to the critter
     * @param factor Multiplier for the critter's speed (0-1)
     * @param duration Time in seconds the effect should last
     */
    void applySlowEffect(float factor, float duration);
    
    /**
     * @brief Registers an observer to receive critter event notifications
     * @param observer Pointer to the observer to register
     */
    void addObserver(CritterObserver* observer);
    
    /**
     * @brief Unregisters an observer from receiving notifications
     * @param observer Pointer to the observer to unregister
     */
    void removeObserver(CritterObserver* observer);
    
    /**
     * @brief Notifies all observers that the critter has reached the end of its path
     */
    void notifyReachedEnd();
    
    /**
     * @brief Notifies all observers that the critter has been defeated
     */
    void notifyDefeated();
};
