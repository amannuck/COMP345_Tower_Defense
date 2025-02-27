//
// Created by amann on 27/02/2025.
//

#include "MapObserver.h"

MapObserver::MapObserver(Map* map) : map(map), offsetX(0), offsetY(0), cellSize(0) {
    map->addObserver(this);

    // Calculate default rendering parameters
    cellSize = std::min(GetScreenWidth() / (map->getWidth() + 2),
                      GetScreenHeight() / (map->getHeight() + 2));
    offsetX = (GetScreenWidth() - (map->getWidth() * cellSize)) / 2;
    offsetY = (GetScreenHeight() - (map->getHeight() * cellSize)) / 2;
}

void MapObserver::update() {
    cellSize = std::min(GetScreenWidth() / (map->getWidth() + 2),
                      GetScreenHeight() / (map->getHeight() + 2));
    offsetX = (GetScreenWidth() - (map->getWidth() * cellSize)) / 2;
    offsetY = (GetScreenHeight() - (map->getHeight() * cellSize)) / 2;

}

void MapObserver::draw() const {
    // Use the map's draw function to render the map
    map->draw(offsetX, offsetY, cellSize);

    // Display observer status
    DrawText("MapObserver: Active", 10, 10, 20, RED);
}
