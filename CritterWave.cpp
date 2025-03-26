// CritterWave.cpp
#include "CritterWave.h"
#include <iostream>

CritterWave::CritterWave(int waveLevel, const std::vector<Vector2>& path)
    : path(path), currentCritterIndex(0), spawnTimer(0), spawnDelay(1.5f), waveLevel(waveLevel) {

    if (path.empty()) {
        std::cerr << "ERROR: Path is empty! Cannot spawn critters." << std::endl;
        return;
    }

    // Use the factory pattern to create critters
    auto factory = CritterFactoryCreator::createFactory(waveLevel);
    int critterCount = 5 + waveLevel;
    critters = factory->createWave(path, critterCount);

    // Set initial position for all critters
    for (auto& critter : critters) {
        critter.setPosition(path.front());
    }

    std::cout << "✅ CritterWave created! " << critters.size() << " critters initialized." << std::endl;
}

CritterWave::CritterWave(int waveLevel, const std::vector<Vector2>& path, int cellSize, int offsetX, int offsetY)
    : path(path), currentCritterIndex(0), spawnTimer(0), spawnDelay(1.5f), waveLevel(waveLevel) {

    if (path.empty()) {
        std::cerr << "ERROR: Path is empty! Cannot spawn critters." << std::endl;
        return;
    }

    // Use the factory pattern to create critters
    auto factory = CritterFactoryCreator::createFactory(waveLevel);
    int critterCount = 5 + waveLevel;
    critters = factory->createWave(path, critterCount);

    // Set initial position for all critters
    for (auto& critter : critters) {
        critter.setPosition(path.front());
        
        // Debug print
        std::cout << "Critter " << currentCritterIndex << " initialized at position: ("
                  << path.front().x << ", " << path.front().y << ")" << std::endl;
    }

    std::cout << "✅ CritterWave created! " << critters.size() << " critters initialized." << std::endl;
}

void CritterWave::update() {
    // Call the version with deltaTime using a default value
    update(GetFrameTime());
}

void CritterWave::removeDeadCritters() {
    critters.erase(
        std::remove_if(critters.begin(), critters.end(),
            [](Critter& critter) {
                return critter.isDead() || critter.reachedEnd();
            }),
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
