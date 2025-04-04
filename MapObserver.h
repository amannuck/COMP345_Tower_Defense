/**
 * @file MapObserver.h
 * @author amann
 * @date 27/02/2025
 * @brief Defines the MapObserver class to monitor Map objects
 * @details This file provides the MapObserver class that implements the Observer
 *          pattern to monitor and respond to changes in Map objects. It handles
 *          recalculating rendering parameters when the map changes.
 */

#ifndef MAPOBSERVER_H
#define MAPOBSERVER_H

#pragma once
#include "IObserver.h"
#include "Map.h"
#include <raylib.h>

/**
 * @class MapObserver
 * @brief Observer class for monitoring Map objects
 * @details Implements the IObserver interface to respond to changes in a Map.
 *          Maintains rendering parameters and provides drawing functionality.
 */
class MapObserver : public IObserver {
private:
    Map* map;     ///< Pointer to the observed Map object
    int offsetX;  ///< X-offset for rendering the map
    int offsetY;  ///< Y-offset for rendering the map
    int cellSize; ///< Size of each map cell in pixels

public:
    /**
     * @brief Constructor for the MapObserver class
     * @param map Pointer to the Map object to observe
     */
    MapObserver(Map* map);
    
    /**
     * @brief Virtual destructor with default implementation
     */
    ~MapObserver() override = default;
    
    /**
     * @brief Update method called when the observed Map changes
     * @details Implements the update method from IObserver interface.
     *          Recalculates rendering parameters when the map is modified.
     */
    void update() override;
    
    /**
     * @brief Renders the observed map
     * @details Draws the map using the calculated rendering parameters.
     */
    void draw() const;
};

#endif //MAPOBSERVER_H
