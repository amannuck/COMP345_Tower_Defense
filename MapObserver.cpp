/**
 * @file MapObserver.cpp
 * @author amann
 * @date 27/02/2025
 * @brief Implementation of the MapObserver class
 * @details This file provides the implementation for the MapObserver class,
 *          which observes and responds to changes in a Map object.
 */

#include "MapObserver.h"

/**
 * @brief Constructor for the MapObserver class
 * @param map Pointer to the Map object to observe
 * @details Initializes the observer and registers it with the map.
 *          Calculates initial rendering parameters based on screen and map dimensions.
 */
MapObserver::MapObserver(Map* map) : map(map), offsetX(0), offsetY(0), cellSize(0) {
    map->addObserver(this);
    // Calculate default rendering parameters
    cellSize = std::min(GetScreenWidth() / (map->getWidth() + 2),
                      GetScreenHeight() / (map->getHeight() + 2));
    offsetX = (GetScreenWidth() - (map->getWidth() * cellSize)) / 2;
    offsetY = (GetScreenHeight() - (map->getHeight() * cellSize)) / 2;
}

/**
 * @brief Update method called when the observed Map changes
 * @details Recalculates rendering parameters to ensure the map is displayed correctly
 *          after any changes to the map or screen dimensions.
 */
void MapObserver::update() {
    cellSize = std::min(GetScreenWidth() / (map->getWidth() + 2),
                      GetScreenHeight() / (map->getHeight() + 2));
    offsetX = (GetScreenWidth() - (map->getWidth() * cellSize)) / 2;
    offsetY = (GetScreenHeight() - (map->getHeight() * cellSize)) / 2;
}

/**
 * @brief Renders the observed map
 * @details Draws the map using the calculated rendering parameters and
 *          displays a status message indicating that the observer is active.
 */
void MapObserver::draw() const {
    // Use the map's draw function to render the map
    map->draw(offsetX, offsetY, cellSize);
    // Display observer status
    DrawText("MapObserver: Active", 10, 10, 20, RED);
}
