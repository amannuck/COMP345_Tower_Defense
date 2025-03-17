//
// Created by amann on 23/02/2025.
//
// Tower.cpp
#include "Tower.h"
#include <cmath>

#include "raymath.h"


// Tower.cpp
Tower::Tower(float range, float power, float fireRate, int buyCost, int upgradeCost,
             const std::string& name, const Color& color, float projectileSpeed)
    : level(1), range(range), power(power), fireRate(fireRate), lastShotTime(0),
      position({0, 0}), buyCost(buyCost), upgradeCost(upgradeCost), name(name), color(color),
      projectileSpeed(projectileSpeed) {}

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
                // Shoot a projectile at the critter
                Vector2 direction = Vector2Subtract(critter.getPosition(), position);
                direction = Vector2Normalize(direction);
                projectiles.push_back({position, Vector2Scale(direction, projectileSpeed), true});
                resetShotTimer();  // Reset the shot timer after shooting
                break;  // Shoot only one critter per shot
            }
        }
    }
}

void Tower::updateProjectiles(std::vector<Critter>& critters) {
    for (auto& projectile : projectiles) {
        if (projectile.active) {
            // Update projectile position (frame-rate independent)
            projectile.position = Vector2Add(projectile.position, Vector2Scale(projectile.velocity, GetFrameTime()));

            // Check if the projectile hits any critter
            for (auto& critter : critters) {
                if (critter.isActive() && !critter.isDead()) {
                    float distance = Vector2Distance(projectile.position, critter.getPosition());
                    if (distance < 10.0f) {  // Assuming a hit radius of 10 pixels
                        critter.takeDamage(power);
                        projectile.active = false;  // Deactivate the projectile after hitting
                        break;
                    }
                }
            }

            // Deactivate the projectile if it goes off-screen
            if (projectile.position.x < 0 || projectile.position.x > GetScreenWidth() ||
                projectile.position.y < 0 || projectile.position.y > GetScreenHeight()) {
                projectile.active = false;
                }
        }
    }

    // Remove inactive projectiles
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& p) { return !p.active; }), projectiles.end());
}

void Tower::draw() const {
    // Draw tower
    DrawCircle(position.x, position.y, 15, color);

    // Draw level indicator
    std::string levelText = "Lvl " + std::to_string(level);
    DrawText(levelText.c_str(), position.x - 10, position.y - 25, 10, BLACK);

    // Draw projectiles
    for (const auto& projectile : projectiles) {
        if (projectile.active) {
            DrawCircle(projectile.position.x, projectile.position.y, 5, RED);  // Draw a small red circle as the projectile
        }
    }
}

bool Tower::canShoot() const {
    return GetTime() - lastShotTime >= 1.0f / fireRate;
}

void Tower::resetShotTimer() {
    lastShotTime = GetTime();
}


BasicTower::BasicTower()
    : Tower(150.0f, 10.0f, 1.0f, 100, 50, "Basic Tower", RED, 3020.0f) {}

void BasicTower::upgrade() {
    Tower::upgrade();
    // Basic tower gets extra power on upgrade
    power *= 1.1f;
}

AreaTower::AreaTower()
    : Tower(120.0f, 8.0f, 0.8f, 150, 75, "Area Tower", BLUE, 300.0f), areaRadius(40.0f) {}

void AreaTower::upgrade() {
    Tower::upgrade();
    areaRadius *= 1.2f;
}


SlowTower::SlowTower()
    : Tower(130.0f, 5.0f, 1.2f, 125, 60, "Slow Tower", YELLOW, 300.0f),
      slowEffect(0.3f), slowDuration(2.0f) {}

void SlowTower::upgrade() {
    Tower::upgrade();
    slowEffect *= 1.15f;
    slowDuration *= 1.1f;
}