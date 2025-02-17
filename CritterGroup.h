/**
 * @file CritterGroup.h
 * @brief Declaration of the CritterGroup class for managing critter waves in the Tower Defense game.
 */

#ifndef CRITTER_GROUP_H
#define CRITTER_GROUP_H

#include <vector>
#include <iostream>
#include <utility>
#include <queue>
#include <cmath>
#include <functional>
#include "critter.h"
#include "mapgen.h"

using namespace std;

/**
 * @class CritterGroup
 * @brief Manages groups of critters and wave generation in the tower defense game.
 *
 * This class is responsible for generating waves of critters, moving them along the map,
 * handling attacks from towers, and managing their lifecycle.
 */
class CritterGroup {
private:
    int waveNum;                  ///< Current wave number
    const Map* map;                ///< Pointer to the game map for pathfinding
    vector<Critter> activeCritters; ///< List of active critters on the map
    queue<Critter> spawnQueue;      ///< Queue of critters waiting to spawn

    /**
     * @brief Calculates critter stats based on the wave number.
     * @param waveNum The current wave number.
     * @return Tuple containing (hp, strength, speed, reward) values.
     */
    tuple<int, int, int, int> calculateCritterStats(int waveNum);

public:
    /**
     * @brief Constructs a CritterGroup object.
     * @param map Pointer to the game map.
     */
    explicit CritterGroup(const Map* map);

    /**
     * @brief Generates a new wave of critters.
     * @return Number of critters in the wave.
     */
    int generateWave();

    /**
     * @brief Moves all active critters along their path.
     * @param onCritterExit Callback function triggered when a critter reaches the exit.
     */
    void moveAllCritters(function<void(int)> onCritterExit);

    /**
     * @brief Spawns the next critter from the queue if available.
     * @return True if a critter was spawned, false if the queue is empty.
     */
    bool spawnNextCritter();

    /**
     * @brief Processes damage to a specific critter in the group.
     * @param critterIndex Index of the critter in activeCritters.
     * @param damage Amount of damage to be applied.
     * @param onCritterDeath Callback function triggered when a critter dies.
     * @return True if the critter died, false otherwise.
     */
    bool processCritterHit(size_t critterIndex, int damage, function<void(int)> onCritterDeath);

    /**
     * @brief Removes dead critters from the active list.
     * @param onCritterDeath Callback function triggered for each removed critter.
     */
    void removeDeadCritters(function<void(int)> onCritterDeath);

    /**
     * @brief Gets the current wave number.
     * @return The current wave number.
     */
    int getCurrentWave() const { return waveNum; }

    /**
     * @brief Gets the list of active critters.
     * @return A reference to the vector of active critters.
     */
    vector<Critter>& getActiveCritters();

    /**
     * @brief Gets the number of critters remaining to spawn.
     * @return Number of critters left in the spawn queue.
     */
    size_t getRemainingSpawns() const { return spawnQueue.size(); }

    /**
     * @brief Checks if the wave is complete (no active or remaining critters).
     * @return True if the wave is complete, false otherwise.
     */
    bool isWaveComplete() const { return spawnQueue.empty() && activeCritters.empty(); }
};

#endif // CRITTER_GROUP_H