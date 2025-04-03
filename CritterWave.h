/**
 * @file CritterWave.h
 * @brief Defines the CritterWave class for managing groups of critters
 * @details This class is responsible for creating, updating, and managing
 *          a wave of critters in the tower defense game. It handles spawning,
 *          movement along paths, and lifecycle management of critters.
 */

#pragma once
#include "Critter.h"
#include "CritterFactory.h"
#include <vector>

/**
 * @class CritterWave
 * @brief Manages a wave of critters and their behavior
 * @details The CritterWave class creates and manages a group of critters that
 *          follow a predefined path. It handles spawning timing, movement updates,
 *          and lifecycle management (removal of dead or exited critters).
 */
class CritterWave {
private:
    std::vector<Critter> critters;         ///< Collection of critters in this wave
    std::vector<Vector2> path;             ///< Waypoints that critters will follow
    int currentCritterIndex;               ///< Index of the next critter to spawn
    float spawnTimer;                      ///< Timer to track when to spawn the next critter
    float spawnDelay;                      ///< Time interval between critter spawns
    int waveLevel;                         ///< Current wave level (affects critter difficulty)

public:
    /**
     * @brief Constructor for CritterWave
     * @param waveLevel The current level of the wave, affects number and difficulty of critters
     * @param path Vector of waypoints that critters will follow
     */
    CritterWave(int waveLevel, const std::vector<Vector2>& path);
    
    /**
     * @brief Constructor for CritterWave with grid cell information
     * @param waveLevel The current level of the wave, affects number and difficulty of critters
     * @param path Vector of waypoints that critters will follow
     * @param cellSize Size of each cell in the grid
     * @param offsetX X-coordinate offset for the grid
     * @param offsetY Y-coordinate offset for the grid
     */
    CritterWave(int waveLevel, const std::vector<Vector2>& path, int cellSize, int offsetX, int offsetY);
    
    /**
     * @brief Default destructor
     */
    ~CritterWave() = default;
    
    /**
     * @brief Update the wave state using the default frame time
     */
    void update();
    
    /**
     * @brief Remove dead critters from the wave
     */
    void removeDeadCritters();
    
    /**
     * @brief Update the wave state with the specified time delta
     * @param deltaTime Time elapsed since the last update
     */
    void update(float deltaTime);
    
    /**
     * @brief Draw all active critters
     */
    void draw() const;
    
    /**
     * @brief Remove critters that have reached the exit
     */
    void removeExitedCritters();
    
    /**
     * @brief Check if all critters in the wave have been spawned
     * @return true if all critters have been spawned, false otherwise
     */
    bool allCrittersDefeated() const;
    
    /**
     * @brief Get a reference to the vector of critters
     * @return Reference to the vector containing all critters in this wave
     */
    std::vector<Critter>& getCritters();
    
    /**
     * @brief Check if the wave is still spawning critters
     * @return true if there are still critters waiting to be spawned, false otherwise
     */
    bool isStillSpawning() const { return currentCritterIndex < critters.size(); }
    
    /**
     * @brief Get the current wave level
     * @return The level of this wave
     */
    int getWaveLevel() const { return waveLevel; }
};
