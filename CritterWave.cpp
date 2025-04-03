/**
 * @file CritterWave.cpp
 * @brief Implementation of the CritterWave class
 * @details This file contains the implementation of the CritterWave class, which
 *          manages a wave of critters including their creation, spawning, movement,
 *          and lifecycle management.
 */

#include "CritterWave.h"
#include <iostream>

/**
 * @brief Constructor for CritterWave
 * @param waveLevel The current level of the wave, affects number and difficulty of critters
 * @param path Vector of waypoints that critters will follow
 * @details Creates a wave of critters using the appropriate factory based on wave level.
 *          The number of critters increases with wave level.
 */
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

/**
 * @brief Constructor for CritterWave with grid cell information
 * @param waveLevel The current level of the wave, affects number and difficulty of critters
 * @param path Vector of waypoints that critters will follow
 * @param cellSize Size of each cell in the grid
 * @param offsetX X-coordinate offset for the grid
 * @param offsetY Y-coordinate offset for the grid
 * @details Creates a wave of critters with additional debug information by printing
 *          the initial position of each critter. Grid parameters can be used for 
 *          position calculations.
 */
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

/**
 * @brief Update the wave state using the default frame time
 * @details Calls the version with deltaTime using GetFrameTime() as the default value
 */
void CritterWave::update() {
    // Call the version with deltaTime using a default value
    update(GetFrameTime());
}

/**
 * @brief Remove dead or finished critters from the wave
 * @details Uses the erase-remove idiom to remove critters that are either dead
 *          or have reached the end of their path
 */
void CritterWave::removeDeadCritters() {
    critters.erase(
        std::remove_if(critters.begin(), critters.end(),
            [](Critter& critter) {
                return critter.isDead() || critter.reachedEnd();
            }),
        critters.end()
    );
}

/**
 * @brief Update the wave state with the specified time delta
 * @param deltaTime Time elapsed since the last update
 * @details Updates the spawn timer, activates new critters when appropriate,
 *          and updates the position of all active critters
 */
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

/**
 * @brief Draw all active critters
 * @details Iterates through all critters and calls their draw method if they are active
 *          Also outputs debug information about the number of critters being drawn
 */
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

/**
 * @brief Check if all critters in the wave have been spawned
 * @return true if all critters have been spawned, false otherwise
 * @details This does not check if critters are dead or have reached the end,
 *          only if they have all been spawned
 */
bool CritterWave::allCrittersDefeated() const {
    return currentCritterIndex >= critters.size();
}

/**
 * @brief Get a reference to the vector of critters
 * @return Reference to the vector containing all critters in this wave
 */
std::vector<Critter>& CritterWave::getCritters() {
    return critters;
}
