#ifndef CRITTER_GROUP_H
#define CRITTER_GROUP_H

using namespace std;
#include <vector>
#include <queue>
#include "critter.h"
#include "mapgen.h"

/**
 * @class CritterGroup
 * @brief Manages groups of critters and wave generation in the tower defense game
 */
class CritterGroup {
    private:
        int waveNum;                  ///< Current wave number
        const Map* map;                  ///< Pointer to the map object
        vector<Critter> activeCritters;  ///< List of active critters
        queue<Critter> spawnQueue;       ///< Queue of critters to spawn

        /**
         * @brief Calculate critter stats based on wave number
         * @param waveNum Current wave number
         * @return tuple of (hp, strength, speed, reward)
         */
        tuple<int, int, int ,int> calculateCritterStats(int waveNum);
    
    public:
        /**
         * @brief Constructor for Critter Group
         * @param map Pointer to the game map
         */
        explicit CritterGroup(const Map* map); 

        /**
         * @brief Generate a new wave of critters
         * @return Number of critters in the wave
         */
        int generateWave();

        /**
         * @brief Move all active critters
         * @param onCritterExit Callback function when critter reaches exit
         */
        void moveAllCritters(function<void(int)> onCritterExit);

        /**
         * @brief Spawn next critter from queue if available
         * @return True if critter was spawned, false if queue empty
         */
        bool spawnNextCritter();

        /**
         * @brief Damage a specific critter in the group
         * @param critterIndex Index of the critter in activecritters
         * @param damage Amount of damage dealt
         * @param onCritterDeath Callback function when critter dies
         * @return True if critter died, false otherwise
         */
        bool processCritterHit(size_t critterIndex, int damage, function<void(int)> onCritterDeath);

        /**
         * @brief Remove dead critters from active list
         * @param onCritterDeath Callback for each removed critter
         */
        void removeDeadCritters(function<void(int)> onCritterDeath);

        // Getters
        int getCurrentWave() const { return waveNum; }
        const vector<Critter>& getActiveCritters() const { return activeCritters; }
        size_t getRemainingSpawns() const { return spawnQueue.size(); }
        bool isWaveComplete() const { return spawnQueue.empty() && activeCritters.empty(); }
};

#endif // CRITTER_GROUP_H