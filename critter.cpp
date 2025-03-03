#include "Critter.h"
#include <cmath>
#include <iostream>
#include <algorithm>  // For std::remove and std::find

Critter::Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path)
    : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp), reward(reward), strength(strength),
      position(path.front()), path(path), pathIndex(0), active(false) {}

void Critter::move() {
    if (!active) return;

    if (pathIndex < path.size() - 1) {
        Vector2 nextPosition = path[pathIndex + 1];

        float dx = nextPosition.x - position.x;
        float dy = nextPosition.y - position.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < speed) {
            pathIndex++;
            position = nextPosition;
        } else {
            position.x += (dx / distance) * speed;
            position.y += (dy / distance) * speed;
        }

        std::cout << "🐜 Critter moving to " << position.x << ", " << position.y << std::endl;
    } else {
        reachedEndFlag = true;
        notifyReachedEnd();  // Notify observers that the critter reached the end
        std::cout << "⚠️ Critter reached the exit!" << std::endl;
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
    return pathIndex >= path.size() - 1;
}

void Critter::draw() const {
    if (hitPoints <= 0) return;

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
    for (auto observer : observers) {
        if (observer) {
            observer->onCritterReachedEnd(*this);
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