#pragma once
#include "Critter.h"
#include <vector>

class CritterWave {
private:
    std::vector<Critter> critters;
    std::vector<Vector2> path;
    int currentCritterIndex;
    float spawnTimer;
    float spawnDelay;

public:
    CritterWave(int waveLevel, const std::vector<Vector2>& path);
    CritterWave(int waveLevel, const std::vector<Vector2>& path, int cellSize, int offsetX, int offsetY);
    ~CritterWave() = default;

    void update();
    void removeDeadCritters();
    void update(float deltaTime);
    void draw() const;
    bool allCrittersDefeated() const;
    std::vector<Critter>& getCritters();
    bool isStillSpawning() const { return currentCritterIndex < critters.size(); }
};
