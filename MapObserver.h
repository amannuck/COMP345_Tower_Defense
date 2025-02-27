//
// Created by amann on 27/02/2025.
//

#ifndef MAPOBSERVER_H
#define MAPOBSERVER_H

// MapObserver.h
#pragma once
#include "IObserver.h"
#include "Map.h"
#include <raylib.h>

class MapObserver : public IObserver {
private:
    Map* map;
    int offsetX;
    int offsetY;
    int cellSize;

public:
    MapObserver(Map* map);
    ~MapObserver() override = default;
    void update() override;
    void draw() const;
};


#endif //MAPOBSERVER_H
