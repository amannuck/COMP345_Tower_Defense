#include "CritterWave.h"
#include <iostream>

CritterWave::CritterWave(int waveLevel, const std::vector<Vector2>& path)
    : path(path), currentCritterIndex(0), spawnTimer(0), spawnDelay(1.5f) {

    if (path.empty()) {
        std::cerr << "ERROR: Path is empty! Cannot spawn critters." << std::endl;
        return;
    }

    int critterCount = 5 + waveLevel;
    for (int i = 0; i < critterCount; i++) {
        float speed = 1.0f + (waveLevel * 0.1f);
        float hp = 50.0f + (waveLevel * 10);
        int reward = 10 + (waveLevel * 2);
        int strength = 2 + waveLevel;

        Critter critter(waveLevel, speed, hp, reward, strength, path);
        critter.setPosition(path.front());
        critters.push_back(critter);
    }

    std::cout << "✅ CritterWave created! " << critters.size() << " critters initialized." << std::endl;
}

CritterWave::CritterWave(int waveLevel, const std::vector<Vector2>& path, int cellSize, int offsetX, int offsetY)
    : path(path), currentCritterIndex(0), spawnTimer(0), spawnDelay(1.5f) {

    if (path.empty()) {
        std::cerr << "ERROR: Path is empty! Cannot spawn critters." << std::endl;
        return;
    }

    int critterCount = 5 + waveLevel;
    for (int i = 0; i < critterCount; i++) {
        float speed = 40.0f + (waveLevel * 0.1f);
        float hp = 50.0f + (waveLevel * 10);
        int reward = 10 + (waveLevel * 2);
        int strength = 2 + waveLevel;

        // Use the screen coordinates directly
        Critter critter(waveLevel, speed, hp, reward, strength, path);

        critter.setPosition(path.front());  // Start at the first path point
        critters.push_back(critter);

        // Debug print
        std::cout << "Critter " << i << " initialized at position: ("
                  << path.front().x << ", " << path.front().y << ")" << std::endl;
    }

    std::cout << "✅ CritterWave created! " << critters.size() << " critters initialized." << std::endl;
}

// Add this function to CritterWave.cpp
void CritterWave::update() {
    // Call the version with deltaTime using a default value
    update(GetFrameTime());
}

void CritterWave::removeDeadCritters() {
    critters.erase(
        std::remove_if(critters.begin(), critters.end(),
            [](Critter& critter) { return critter.isDead(); }),
        critters.end()
    );
}

void CritterWave::update(float deltaTime) {
    // Spawn new critters based on timer
    if (currentCritterIndex < critters.size()) {
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay) {
            // Activate the next critter
            critters[currentCritterIndex].activate();
            currentCritterIndex++;
            spawnTimer = 0;
        }
    }

    // Update all active critters
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead() && !critter.reachedEnd()) {
            critter.move();
        }
    }
}

void CritterWave::draw() const {
    int drawnCritters = 0;
    for (const auto& critter : critters) {
        if (critter.isActive()) {
            critter.draw();
            drawnCritters++;
        }
    }
    std::cout << "🎨 Drawing " << drawnCritters << " critters." << std::endl;
}



bool CritterWave::allCrittersDefeated() const {
    return currentCritterIndex >= critters.size();
}

std::vector<Critter>& CritterWave::getCritters() {
    return critters;
}
