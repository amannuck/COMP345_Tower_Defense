//#include "Critter.h"
//#include <cmath>
//
//Critter::Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path)
//        : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp), reward(reward), strength(strength),
//          position(path.front()), path(path), pathIndex(0), active(false) {}
//
//void Critter::move() {
//    if (!active) return;  // ✅ Only move if activated
//
//    if (pathIndex < path.size() - 1) {  // ✅ Ensure pathIndex is within range
//        Vector2 nextPosition = path[pathIndex + 1];
//
//        float dx = nextPosition.x - position.x;
//        float dy = nextPosition.y - position.y;
//        float distance = sqrt(dx * dx + dy * dy);
//
//        if (distance < speed) {  // ✅ Snap to next point
//            pathIndex++;
//            position = nextPosition;
//        } else {  // ✅ Move towards the next point
//            position.x += (dx / distance) * speed;
//            position.y += (dy / distance) * speed;
//        }
//    } else {
//        reachedEndFlag = true;  // ✅ Mark critter as reaching the exit
//    }
//}
//
//void Critter::takeDamage(float damage) {
//    hitPoints -= damage;
//}
//
//bool Critter::isDead() const {
//    return hitPoints <= 0;
//}
//
//bool Critter::reachedEnd() const {
//    return pathIndex >= path.size() - 1;
//}
//
//void Critter::draw() const {
//    Color critterColor = { 255, 0, 0, 255 }; // Red for critters
//    DrawCircleV(position, 10, critterColor);
//
//    if (hitPoints > 0) {  // ✅ Draw health bar only if alive
//        float healthBarWidth = 20 * (hitPoints / maxHitPoints);
//        DrawRectangle(position.x - 10, position.y - 15, healthBarWidth, 5, GREEN);
//    }
//}
//
//void Critter::activate() {  // ✅ Ensure this is correctly defined
//    active = true;
//}

#include "Critter.h"
#include <cmath>
#include <iostream>

//Critter::Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path)
//        : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp), reward(reward), strength(strength),
//          position(path.front()), path(path), pathIndex(0), active(false) {}


Critter::Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path)
        : level(level), speed(speed), hitPoints(hp), maxHitPoints(hp), reward(reward), strength(strength),
          position(path.front()), path(path), pathIndex(0), active(false) {}

//void Critter::move() {
//    if (!active) return;
//
//    if (pathIndex < path.size() - 1) {
//        Vector2 nextPosition = path[pathIndex + 1];
//
//        float dx = nextPosition.x - position.x;
//        float dy = nextPosition.y - position.y;
//        float distance = sqrt(dx * dx + dy * dy);
//
//        if (distance < speed) {
//            pathIndex++;
//            position = nextPosition;
//        } else {
//            position.x += (dx / distance) * speed;
//            position.y += (dy / distance) * speed;
//        }
//    } else {
//        reachedEndFlag = true;
//    }
//}

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

        // ✅ Debugging output (Only if needed)
        std::cout << "🐜 Critter moving to " << position.x << ", " << position.y << std::endl;
    } else {
        reachedEndFlag = true;
        std::cout << "⚠️ Critter reached the exit!" << std::endl;
    }
}


void Critter::takeDamage(float damage) {
    hitPoints -= damage;
}

bool Critter::isDead() const {
    return hitPoints <= 0;
}

bool Critter::reachedEnd() const {
    return pathIndex >= path.size() - 1;
}

//void Critter::draw() const {
//    if (hitPoints <= 0) return;  // Ensure critters with 0 HP are not drawn
//
//    Color critterColor = { 255, 0, 0, 255 };
//
//    // Debugging Output
//    std::cout << "🖌 Drawing critter at (" << position.x << ", " << position.y << ")" << std::endl;
//
//    // Draw the critter
//    DrawCircleV(position, 10, critterColor);
//
//    // Draw health bar if still alive
//    if (hitPoints > 0) {
//        float healthBarWidth = 20 * ((float)hitPoints / (float)maxHitPoints);  // Ensure correct division
//        DrawRectangle(position.x - 10, position.y - 15, healthBarWidth, 5, GREEN);
//    }
//}

void Critter::draw() const {
    if (hitPoints <= 0) return;

    // Assume that the critter's position is in grid coordinates,
    // and convert it to screen coordinates.
    // For example, if cellSize and offset are known (you might pass them as parameters
    // or have them globally accessible), do:
    // Vector2 screenPos = { position.x * cellSize + offsetX, position.y * cellSize + offsetY };
    // For now, we assume positions are already in screen coordinates.
    DrawCircleV(position, 10, RED);

    float healthBarWidth = 20 * ((float)hitPoints / (float)maxHitPoints);
    DrawRectangle(position.x - 10, position.y - 15, healthBarWidth, 5, GREEN);
}


void Critter::activate() {
    active = true;
}
