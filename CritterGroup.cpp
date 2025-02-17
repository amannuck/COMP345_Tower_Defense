/**
 * @file CritterGroup.cpp
 * @brief Implementation of the CritterGroup class for managing critter waves in the Tower Defense game.
 */

#include "CritterGroup.h"
#include <cmath>
#include <algorithm>

/**
 * @brief Constructs a CritterGroup object associated with a given map.
 *
 * @param map Pointer to the game map for critter pathfinding.
 */
CritterGroup::CritterGroup(const Map* map)
        : waveNum(0), map(map) {
}

/**
 * @brief Calculates the stats for a critter based on the wave number.
 *
 * @param waveNum The current wave number.
 * @return A tuple containing (hp, strength, speed, reward) values.
 */
std::tuple<int, int, int, int> CritterGroup::calculateCritterStats(int waveNum) {
    // Base stats
    double baseHP = 100.0;
    double baseStrength = 10.0;
    double baseSpeed = 1.0;
    double baseReward = 20.0;

    // Scale stats with wave number
    double scaleFactor = 1.0 + (waveNum * 0.1); // 10% increase per wave

    int hp = static_cast<int>(std::lround(baseHP * scaleFactor));
    int strength = static_cast<int>(std::lround(baseStrength * scaleFactor));
    int speed = static_cast<int>(std::lround(baseSpeed + (waveNum / 5.0))); // Increase speed every 5 waves
    int reward = static_cast<int>(std::lround(baseReward * scaleFactor));

    // Ensure minimum values
    hp = max(hp, 1);
    strength = max(strength, 1);
    speed = max(speed, 1);
    reward = max(reward, 1);

    return make_tuple(hp, strength, speed, reward);
}

/**
 * @brief Generates a new wave of critters.
 *
 * @return The number of critters in the generated wave.
 */
int CritterGroup::generateWave() {
    waveNum++;

    int numCritters = 5 + (waveNum * 2); // Base 5 critters + 2 per wave

    activeCritters.clear();
    while (!spawnQueue.empty()) {
        spawnQueue.pop();
    }

    pair<int, int> entryPoint = map->getEntry();

    for (int i = 0; i < numCritters; i++) {
        auto [hp, strength, speed, reward] = calculateCritterStats(waveNum);
        Critter newCritter(hp, strength, speed, waveNum, reward, entryPoint, map);
        spawnQueue.push(newCritter);
    }

    return numCritters;
}

/**
 * @brief Moves all active critters along their path.
 *
 * @param onCritterExit Callback function when a critter reaches the exit.
 */
void CritterGroup::moveAllCritters(std::function<void(int)> onCritterExit) {
    for (auto it = activeCritters.begin(); it != activeCritters.end();) {
        it->move();
        if (it->hasReachedExit()) {
            onCritterExit(it->getStrength());
            it = activeCritters.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * @brief Spawns the next critter from the queue if available.
 *
 * @return True if a critter was spawned, false if the queue is empty.
 */
bool CritterGroup::spawnNextCritter() {
    if (spawnQueue.empty()) {
        return false;
    }

    activeCritters.push_back(spawnQueue.front());
    spawnQueue.pop();
    return true;
}

/**
 * @brief Processes damage on a specific critter in the group.
 *
 * @param critterIndex Index of the critter in the activeCritters vector.
 * @param damage The amount of damage to apply.
 * @param onCritterDeath Callback function when a critter dies.
 * @return True if the critter died, false otherwise.
 */
bool CritterGroup::processCritterHit(size_t critterIndex, int damage, std::function<void(int)> onCritterDeath) {
    if (critterIndex >= activeCritters.size()) {
        return false;
    }

    activeCritters[critterIndex].takeDamage(damage);

    if (activeCritters[critterIndex].isDead()) {
        onCritterDeath(activeCritters[critterIndex].getReward());
        return true;
    }

    return false;
}

/**
 * @brief Removes dead critters from the active list.
 *
 * @param onCritterDeath Callback function for each removed critter.
 */
void CritterGroup::removeDeadCritters(std::function<void(int)> onCritterDeath) {
    auto it = activeCritters.begin();
    while (it != activeCritters.end()) {
        if (it->isDead()) {
            onCritterDeath(it->getReward());
            it = activeCritters.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * @brief Gets the list of active critters.
 *
 * @return A reference to the vector of active critters.
 */
vector<Critter>& CritterGroup::getActiveCritters() {
    return activeCritters;
}
