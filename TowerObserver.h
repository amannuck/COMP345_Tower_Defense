/**
 * @file TowerObserver.h
 * @brief Defines interfaces for the Observer pattern for tower events
 * @details This file implements the Observer design pattern specifically for tower events,
 *          allowing components to monitor and react to tower-related activities.
 */

#pragma once
#include <raylib.h>
#include <string>

// Forward declarations
class Tower;

/**
 * @enum TowerEventType
 * @brief Defines the possible types of tower events
 */
enum class TowerEventType {
    TOWER_FIRED,    ///< Tower has fired at a target
    TOWER_UPGRADED, ///< Tower has been upgraded
    TOWER_SOLD,     ///< Tower has been sold
    TOWER_PLACED    ///< Tower has been placed on the map
};

/**
 * @class ITowerObserver
 * @brief Interface for objects that want to observe tower events
 * @details Classes implementing this interface can register with tower subjects
 *          to receive notifications about tower events.
 */
class ITowerObserver {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~ITowerObserver() = default;
    
    /**
     * @brief Callback method invoked when a tower event occurs
     * @param tower Pointer to the tower that triggered the event
     * @param eventType Type of event that occurred
     * @details This method is called by the tower subject when a tower event occurs.
     *          Implementations should handle the specific event type appropriately.
     */
    virtual void onTowerEvent(Tower* tower, TowerEventType eventType) = 0;
};

/**
 * @class ITowerSubject
 * @brief Interface for objects that generate tower events
 * @details Classes implementing this interface can maintain a list of observers
 *          and notify them when tower events occur.
 */
class ITowerSubject {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~ITowerSubject() = default;
    
    /**
     * @brief Registers an observer to receive notifications
     * @param observer Pointer to the observer object
     */
    virtual void addObserver(ITowerObserver* observer) = 0;
    
    /**
     * @brief Unregisters an observer from receiving notifications
     * @param observer Pointer to the observer object to remove
     */
    virtual void removeObserver(ITowerObserver* observer) = 0;
    
    /**
     * @brief Notifies all registered observers of a tower event
     * @param eventType Type of event that occurred
     */
    virtual void notifyObservers(TowerEventType eventType) = 0;
};
