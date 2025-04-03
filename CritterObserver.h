/**
 * @file CritterObserver.h
 * @author amann
 * @date 02/03/2025
 * @brief Observer interface for tracking critter events
 * @details This file defines the CritterObserver interface that implements the Observer
 *          pattern to monitor and react to events related to critters, such as when
 *          they reach the end of a path or are defeated.
 */

#ifndef CRITTEROBSERVER_H
#define CRITTEROBSERVER_H
#pragma once
#include "IObserver.h"

// Forward declaration to avoid circular dependencies
class Critter;

/**
 * @class CritterObserver
 * @brief Observer interface for monitoring critter-related events
 * @details This class extends the IObserver interface to provide specific
 *          callbacks for critter events in the tower defense game. Classes that
 *          need to respond to critter events (like game controllers, UI elements,
 *          or scoring systems) can implement this interface.
 */
class CritterObserver: public IObserver {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~CritterObserver() = default;
    
    /**
     * @brief Notification when a critter reaches the end of its path
     * @param critter Reference to the critter that reached the end
     * @details This method is called when a critter successfully navigates to the
     *          end of its path. Implementations might handle player health reduction,
     *          scoring updates, or game state changes.
     */
    virtual void onCritterReachedEnd(const Critter& critter) = 0;
    
    /**
     * @brief Notification when a critter is defeated
     * @param critter Reference to the critter that was defeated
     * @details This method is called when a critter is defeated by towers or other game
     *          mechanics. Implementations might handle resource gathering, scoring updates,
     *          or special effects triggered by critter defeat.
     */
    virtual void onCritterDefeated(const Critter& critter) = 0;
};

#endif //CRITTEROBSERVER_H
