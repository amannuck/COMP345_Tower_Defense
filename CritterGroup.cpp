#include "CritterGroup.h"
#include <cmath>
#include <algorithm>

CritterGroup::CritterGroup(const Map* map) 
    : waveNum(0), map(map) {
}

tuple<int, int, int, int> CritterGroup::calculateCritterStats(int waveNum) {
    // Base stats
    int baseHP = 100;
    int baseStrength = 10;
    int baseSpeed = 1;
    int baseReward = 20;

    // Scale stats with wave number
    float scaleFactor = 1.0f + (waveNum * 0.1f); // 10% increase per wave
    
    int hp = static_cast<int>(baseHP * scaleFactor);
    int strength = static_cast<int>(baseStrength * scaleFactor);
    int speed = baseSpeed + (waveNum / 5); // Increase speed every 5 waves
    int reward = static_cast<int>(baseReward * scaleFactor);

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

void CritterGroup::moveAllCritters(function<void(int)> onCritterExit) {
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

bool CritterGroup::processCritterHit(size_t critterIndex, int damage, function<void(int)> onCritterDeath) {
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

void CritterGroup::removeDeadCritters(function<void(int)> onCritterDeath) {
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