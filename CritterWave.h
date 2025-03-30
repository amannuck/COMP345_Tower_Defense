// CritterWave.h
#pragma once
#include "Critter.h"
#include "CritterFactory.h"
#include <vector>

class CritterWave {
private:
    std::vector<Critter> critters;
    std::vector<Vector2> path;
    int currentCritterIndex;
    float spawnTimer;
    float spawnDelay;
    int waveLevel;  // Added waveLevel field to track current wave

public:
    CritterWave(int waveLevel, const std::vector<Vector2>& path);
    CritterWave(int waveLevel, const std::vector<Vector2>& path, int cellSize, int offsetX, int offsetY);
    ~CritterWave() = default;

    void update();
    void removeDeadCritters();
    void update(float deltaTime);
    void draw() const;
    void removeExitedCritters(); 
    bool allCrittersDefeated() const;
    std::vector<Critter>& getCritters();
    bool isStillSpawning() const { return currentCritterIndex < critters.size(); }
    int getWaveLevel() const { return waveLevel; }  // Added getter for wave level
};
