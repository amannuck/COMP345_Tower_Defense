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
    ~CritterWave() = default;

    void update();
    void update(float deltaTime);
    void draw(int offsetX = 0, int offsetY = 0, int cellSize = 1) const;
    bool allCrittersDefeated() const;
    std::vector<Critter>& getCritters();
    bool isStillSpawning() const { return currentCritterIndex < critters.size(); }
};
