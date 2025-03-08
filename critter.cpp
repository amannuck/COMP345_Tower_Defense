#include "Critter.h"
#include <cmath>
#include <iostream>
#include <algorithm>  // For std::remove and std::find

#include "raymath.h"

Critter::Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path)
    : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp), reward(reward), strength(strength),
      position(path.front()), path(path), pathIndex(0), active(false) {}


void Critter::move() {
    if (pathIndex < path.size() - 1) {
        Vector2 target = path[pathIndex + 1];
        //std::cout << "Moving critter to target: (" << target.x << ", " << target.y << ")" << std::endl;

        Vector2 direction = Vector2Subtract(target, position);
        direction = Vector2Normalize(direction);

        position = Vector2Add(position, Vector2Scale(direction, speed * GetFrameTime()));

        if (Vector2Distance(position, target) < 1.0f) {
            pathIndex++;
            std::cout << "Critter reached path point " << pathIndex << " of " << path.size() - 1 << std::endl;
            
            // Check if we've reached the final path point
            if (pathIndex >= path.size() - 1) {
                std::cout << "🚀 Critter reached the exit!" << std::endl;
                reachedEndFlag = true;
                active = false;
                notifyReachedEnd();
            }
        }
    } else if (!reachedEndFlag) {
        std::cout << "🚀 Critter reached the exit!" << std::endl;
        reachedEndFlag = true;
        active = false;
        notifyReachedEnd();
    }
}


void Critter::takeDamage(float damage) {
    hitPoints -= damage;
    if (isDead()) {
        notifyDefeated();  // Notify observers that the critter is defeated
    }
}


bool Critter::isDead() const {
    return hitPoints <= 0;
}

bool Critter::reachedEnd() const {
    return reachedEndFlag || pathIndex >= path.size() - 1;
}

void Critter::draw() const {
    if (hitPoints <= 0) return;

    // Debug print
    //std::cout << "Drawing critter at position: (" << position.x << ", " << position.y << ")" << std::endl;

    DrawCircleV(position, 10, RED);

    float healthBarWidth = 20 * ((float)hitPoints / (float)maxHitPoints);
    DrawRectangle(position.x - 10, position.y - 15, healthBarWidth, 5, GREEN);
}

void Critter::activate() {
    active = true;
}

void Critter::addObserver(CritterObserver* observer) {
    if (!observer) {
        std::cerr << "ERROR: Attempted to add a null observer." << std::endl;
        return;
    }

    if (std::find(observers.begin(), observers.end(), observer) == observers.end()) {
        observers.push_back(observer);
        std::cout << "Observer added successfully." << std::endl;
    } else {
        std::cerr << "WARNING: Observer already exists in the list." << std::endl;
    }
}

void Critter::removeObserver(CritterObserver* observer) {
    if (!observer) {
        std::cerr << "ERROR: Attempted to remove a null observer." << std::endl;
        return;
    }

    auto it = std::remove(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it, observers.end());
        std::cout << "Observer removed successfully." << std::endl;
    } else {
        std::cerr << "WARNING: Observer not found in the list." << std::endl;
    }
}

void Critter::notifyReachedEnd() {
    std::cout << "Notifying " << observers.size() << " observers that critter reached end" << std::endl;
    for (auto observer : observers) {
        if (observer) {
            std::cout << "Calling onCritterReachedEnd for observer" << std::endl;
            observer->onCritterReachedEnd(*this);
        } else {
            std::cerr << "ERROR: Null observer in the list!" << std::endl;
        }
    }
}

void Critter::notifyDefeated() {
    for (auto observer : observers) {
        if (observer) {
            observer->onCritterDefeated(*this);
        }
    }
}