#include "critter.h"

// Constructor: Initialize critter with given stats and position
Critter::Critter(int hp, int str, int spd, int lvl, int rwd, pair<int, int> pos, const Map* gameMap) {
    hitPoints = hp;
    strength = str;
    speed = spd;
    level = lvl;
    reward = rwd;
    position = pos;
    reachedExit = false;
    map = gameMap;
}

// Move the critter towards the exit point
void Critter::move() {
    if (reachedExit || isDead()) {
        return;
    }

    pair<int, int> exitPoint = map->getExit();

    // Move towards exit point
    for (int moves = 0; moves < speed; moves++) {
        // Check if we are at the exit
        if(position.first == exitPoint.first && position.second == exitPoint.second) {
            reachedExit = true;
            return;
        }

        // Move one unit closer to the exit
        pair<int, int> nextPosition = position;
        if (position.first < exitPoint.first && map->isPath(position.first + 1, position.second)) { // move to the right if possible
            nextPosition.first++;
        }
        else if (position.second < exitPoint.second && map->isPath(position.first, position.second + 1)) { // move down if possible
            nextPosition.second++;
        }
        else if (position.second > exitPoint.second && map->isPath(position.first, position.second - 1)) { // move up if possible
            nextPosition.second--;
        }
        else {
            break; // no valid moves
        }

        // Update position
        position = nextPosition;
    }

}

// Reduce hit points by the given damage amount
void Critter::takeDamage(int damage) {
    hitPoints -= damage;
}

// Check if critter is dead (hit points <= 0)
bool Critter::isDead() const {
    return hitPoints <= 0;
}

// Getter and setter methods
int Critter::getHitPoints() const {
    return hitPoints;
}
int Critter::getStrength() const {
    return strength;
}
int Critter::getSpeed() const {
    return speed;
}
int Critter::getLevel() const {
    return level;
}
int Critter::getReward() const {
    return reward;
}
bool Critter::hasReachedExit() const {
    return reachedExit;
}
pair<int, int> Critter::getPosition() const {
    return position;
}
void Critter::setPosition(int x, int y) {
    position = make_pair(x, y);
}
