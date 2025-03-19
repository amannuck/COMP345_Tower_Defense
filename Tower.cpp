//
// Created by amann on 23/02/2025.
//
// Tower.cpp
#include "Tower.h"
#include <cmath>

#include "raymath.h"

Tower::Tower(float range, float power, float fireRate, int buyCost, int upgradeCost,
             const std::string& name, const Color& color)
    : level(1), range(range), power(power), fireRate(fireRate), lastShotTime(0),
      position({0, 0}), buyCost(buyCost), upgradeCost(upgradeCost), name(name), color(color) {}

Tower::~Tower() {}


void Tower::upgrade() {
    level++;
    range *= 1.2f;
    power *= 1.3f;
    fireRate *= 1.1f;
    upgradeCost = static_cast<int>(upgradeCost * 1.5f);
}

int Tower::getRefundValue() const {
    float totalCost = buyCost;
    float refundRatio = 0.7f;  // 70% refund
    return static_cast<int>(totalCost * refundRatio);
}

void Tower::attackCritters(std::vector<Critter>& critters) {
    if (!canShoot()) return;  // Only attack if the tower can shoot

    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            // Check if the critter is within range
            float distance = Vector2Distance(position, critter.getPosition());
            if (distance <= range) {
                // Attack the critter
                critter.takeDamage(power);
                resetShotTimer();  // Reset the shot timer after attacking
                break;  // Attack only one critter per shot
            }
        }
    }
}

void Tower::draw() const {
    // Draw tower
    DrawCircle(position.x, position.y, 15, color);

    // Draw level indicator
    std::string levelText = "Lvl " + std::to_string(level);
    DrawText(levelText.c_str(), position.x - 10, position.y - 25, 10, BLACK);
    

}

bool Tower::canShoot() const {
    return GetTime() - lastShotTime >= 1.0f / fireRate;
}

void Tower::resetShotTimer() {
    lastShotTime = GetTime();
}



BasicTower::BasicTower()
    : Tower(150.0f, 10.0f, 1.0f, 100, 50, "Basic Tower", RED) {}

void BasicTower::upgrade() {
    Tower::upgrade();
    // Basic tower gets extra power on upgrade
    power *= 1.1f;
}

AreaTower::AreaTower()
    : Tower(120.0f, 8.0f, 0.8f, 150, 75, "Area Tower", BLUE), areaRadius(40.0f) {}

void AreaTower::upgrade() {
    Tower::upgrade();
    areaRadius *= 1.2f;
}


SlowTower::SlowTower()
    : Tower(130.0f, 5.0f, 1.2f, 125, 60, "Slow Tower", YELLOW),
      slowEffect(0.3f), slowDuration(2.0f) {}

void SlowTower::upgrade() {
    Tower::upgrade();
    slowEffect *= 1.15f;
    slowDuration *= 1.1f;
}