#include "CritterGroup.h"
#include <cmath>
#include <algorithm>


CritterGroup::CritterGroup(const Map* map)
    : waveNum(0), map(map) {
}

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

int CritterGroup::generateWave() {
    waveNum++;

    // Calculate number of critters for this wave
    int numCritters = 5 + (waveNum * 2); // Base 5 critters + 2 per wave

    // Clear any remaining critters from previous wave
    activeCritters.clear();
    while (!spawnQueue.empty()) {
        spawnQueue.pop();
    }

    // Get entry point from map
    pair<int, int> entryPoint = map->getEntry();

    // Generate critters with scaled stats
    for (int i = 0; i < numCritters; i++) {
        auto [hp, strength, speed, reward] = calculateCritterStats(waveNum);

        // Create critter with calculated stats
        Critter newCritter(
            hp,            // Hit Points
            strength,      // Strength
            speed,         // Speed
            waveNum,       // Level
            reward,        // Reward
            entryPoint,    // Starting position
            map            // Map reference
        );

        // Add to spawn queue
        spawnQueue.push(newCritter);
    }

    return numCritters;
}

void CritterGroup::moveAllCritters(std::function<void(int)> onCritterExit) {
    for (auto it = activeCritters.begin(); it != activeCritters.end();) {
        // Move the critter
        it->move();

        // Check if critter reached exit
        if (it->hasReachedExit()) {
            onCritterExit(it->getStrength());
            it = activeCritters.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool CritterGroup::spawnNextCritter() {
    if (spawnQueue.empty()) {
        return false;
    }

    // Add next critter from queue to active critters
    activeCritters.push_back(spawnQueue.front());
    spawnQueue.pop();
    return true;
}

bool CritterGroup::processCritterHit(size_t critterIndex, int damage, std::function<void(int)> onCritterDeath) {
    if (critterIndex >= activeCritters.size()) {
        return false;
    }

    // Apply damage to critter
    activeCritters[critterIndex].takeDamage(damage);

    // Check if critter died
    if (activeCritters[critterIndex].isDead()) {
        onCritterDeath(activeCritters[critterIndex].getReward());
        return true;
    }

    return false;
}

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