#include "Critter.h"
#include <cmath>
#include <iostream>
#include <algorithm>  // For std::remove and std::find

#include "raymath.h"

Critter::Critter(int level, float speed, float hp, int reward, int strength,
                 const std::vector<Vector2>& path, const std::string& type)
    : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp),
      reward(reward), strength(strength), position(path.front()),
      path(path), pathIndex(0), active(false), type(type) {}

void Critter::applySlowEffect(float factor, float duration) {
    if (factor < slowFactor || (factor == slowFactor && duration > slowDuration)) {
        slowFactor = factor;
        slowDuration = duration;
    }
}

void Critter::move() {
    if (slowDuration > 0) {
        slowDuration -= GetFrameTime();
        if (slowDuration <= 0) {
            slowFactor = 1.0f;
        }
    }

    if (pathIndex < path.size() - 1) {
        Vector2 target = path[pathIndex + 1];
        Vector2 direction = Vector2Subtract(target, position);
        direction = Vector2Normalize(direction);

        float effectiveSpeed = speed * slowFactor;
        position = Vector2Add(position, Vector2Scale(direction, effectiveSpeed * GetFrameTime()));

        if (Vector2Distance(position, target) < 1.0f) {
            pathIndex++;
            if (pathIndex >= path.size() - 1) {
                reachedEndFlag = true;
                active = false;
                notifyReachedEnd();
            }
        }
    } else if (!reachedEndFlag) {
        reachedEndFlag = true;
        active = false;
        notifyReachedEnd();
    }
}

void Critter::takeDamage(float damage) {
    hitPoints -= damage;
    if (isDead()) {
        notifyDefeated();
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

    // Different colors for different critter types
    Color critterColor;
    if (type == "Basic") critterColor = RED;
    else if (type == "Fast") critterColor = GREEN;
    else if (type == "Tank") critterColor = GRAY;
    else if (type == "Boss") critterColor = PURPLE;
    else critterColor = ORANGE;  // Default color for unknown types

    DrawCircleV(position, 10, critterColor);

    // Health bar
    float healthBarWidth = 20 * ((float)hitPoints / (float)maxHitPoints);
    DrawRectangle(position.x - 10, position.y - 15, healthBarWidth, 5, GREEN);

    // Slow effect indicator
    if (slowFactor < 1.0f) {
        DrawCircleV(position, 13, ColorAlpha(BLUE, 0.5f));
        DrawText("SLOW", position.x - 15, position.y - 25, 10, BLUE);
    }

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
