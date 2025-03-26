// Tower.cpp
#include "Tower.h"
#include <cmath>
#include "raymath.h"

Tower::Tower(float range, float power, float fireRate, int buyCost, int upgradeCost,
             const std::string& name, const Color& color)
    : level(1), range(range), power(power), fireRate(fireRate), lastShotTime(0),
      position({0, 0}), buyCost(buyCost), upgradeCost(upgradeCost), name(name), color(color) {
    // Default to FirstInRangeStrategy
    targetingStrategy = std::make_unique<FirstInRangeStrategy>();
}

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

// Add this static member to track active shots
static std::vector<TowerShot> activeShots;

void UpdateTowerShots(float deltaTime) {
    for (auto& shot : activeShots) {
        if (!shot.hit) {
            // Move shot toward target
            Vector2 direction = Vector2Subtract(shot.end, shot.currentPos);
            float distance = Vector2Length(direction);

            if (distance < 5.0f) { // Close enough to count as hit
                shot.hit = true;
            } else {
                direction = Vector2Normalize(direction);
                shot.currentPos = Vector2Add(shot.currentPos,
                    Vector2Scale(direction, shot.speed * deltaTime));
            }
        }

        shot.timer -= deltaTime;
    }

    // Remove expired shots
    activeShots.erase(std::remove_if(activeShots.begin(), activeShots.end(),
        [](const TowerShot& shot) { return shot.timer <= 0; }),
        activeShots.end());
}

void DrawTowerShots() {
    for (const auto& shot : activeShots) {
        // Calculate triangle points (pointing toward target)
        Vector2 direction = Vector2Subtract(shot.end, shot.start);
        direction = Vector2Normalize(direction);

        Vector2 perpendicular = { -direction.y, direction.x };

        float size = 8.0f;
        Vector2 tip = shot.currentPos;
        Vector2 base1 = Vector2Add(tip, Vector2Scale(direction, -size));
        base1 = Vector2Add(base1, Vector2Scale(perpendicular, size/2));
        Vector2 base2 = Vector2Add(tip, Vector2Scale(direction, -size));
        base2 = Vector2Subtract(base2, Vector2Scale(perpendicular, size/2));

        // Draw with fade-out effect
        Color drawColor = shot.color;
        drawColor.a = static_cast<unsigned char>(255 * (shot.timer / 0.5f));

        DrawTriangle(tip, base1, base2, drawColor);

        // Optional: draw a trail
        if (!shot.hit) {
            DrawLineEx(shot.start, shot.currentPos, 2.0f,
                ColorAlpha(drawColor, 0.3f));
        }
    }
}

void Tower::attackCritters(std::vector<Critter>& critters) {
    if (!canShoot()) return;

    Critter* target = targetingStrategy->selectTarget(critters, position, range);

    if (target) {
        // Create a new shot
        TowerShot shot;
        shot.start = position;
        shot.end = target->getPosition();
        shot.currentPos = position;
        shot.speed = 700.0f; // pixels per second
        shot.color = color;
        shot.timer = 0.5f; // seconds to live
        shot.hit = false;

        // Add to active shots
        activeShots.push_back(shot);

        // Apply damage immediately
        target->takeDamage(power);

        resetShotTimer();
    }
}

void Tower::draw() const {
    // Draw tower
    DrawCircle(position.x, position.y, 15, color);

    // Draw level indicator
    std::string levelText = "Lvl " + std::to_string(level);
    DrawText(levelText.c_str(), position.x - 10, position.y - 25, 10, BLACK);

    // If you want to draw shots here instead of in Game.cpp
    DrawTowerShots();
}

bool Tower::canShoot() const {
    return GetTime() - lastShotTime >= 1.0f / fireRate;
}

void Tower::resetShotTimer() {
    lastShotTime = GetTime();
}

// BasicTower implementation
BasicTower::BasicTower()
    : Tower(150.0f, 10.0f, 1.0f, 100, 50, "Basic Tower", RED) {
    // Basic tower uses nearest critter strategy
    setTargetingStrategy(std::make_unique<NearestCritterStrategy>());
}

void BasicTower::upgrade() {
    Tower::upgrade();
    // Basic tower gets extra power on upgrade
    power *= 1.1f;
}

// AreaTower implementation
AreaTower::AreaTower()
    : Tower(120.0f, 8.0f, 0.8f, 150, 75, "Area Tower", BLUE), areaRadius(40.0f) {
    // Area tower uses first in range strategy
    setTargetingStrategy(std::make_unique<FirstInRangeStrategy>());
}

void AreaTower::upgrade() {
    Tower::upgrade();
    areaRadius *= 1.2f;
}

void AreaTower::attackCritters(std::vector<Critter>& critters) {
    if (!canShoot()) return;
    
    Critter* mainTarget = targetingStrategy->selectTarget(critters, position, range);

    if (mainTarget) {
        Vector2 impactPoint = mainTarget->getPosition();
        int hitCount = 0;

        // Create main shot
        TowerShot mainShot;
        mainShot.start = position;
        mainShot.end = impactPoint;
        mainShot.currentPos = position;
        mainShot.speed = 600.0f; // Slightly slower for area effect
        mainShot.color = color;
        mainShot.timer = 0.5f;
        mainShot.hit = false;
        activeShots.push_back(mainShot);

        // Damage all critters in the area
        for (auto& critter : critters) {
            if (critter.isActive() && !critter.isDead()) {
                float distance = Vector2Distance(impactPoint, critter.getPosition());
                if (distance <= areaRadius) {
                    critter.takeDamage(power);
                    hitCount++;

                    // Create secondary shot for each hit critter
                    TowerShot secondaryShot;
                    secondaryShot.start = impactPoint;
                    secondaryShot.end = critter.getPosition();
                    secondaryShot.currentPos = impactPoint;
                    secondaryShot.speed = 700.0f;
                    secondaryShot.color = SKYBLUE;
                    secondaryShot.timer = 0.3f;
                    secondaryShot.hit = false;
                    activeShots.push_back(secondaryShot);
                }
            }
        }

        if (hitCount > 0) {
            // Draw area attack effect
            DrawCircleV(impactPoint, areaRadius, ColorAlpha(BLUE, 0.3f));
            DrawCircleLines(impactPoint.x, impactPoint.y, areaRadius, BLUE);
            
            resetShotTimer();
        }
    }
}

// SlowTower implementation
SlowTower::SlowTower()
    : Tower(130.0f, 5.0f, 1.2f, 125, 60, "Slow Tower", YELLOW),
      slowEffect(0.3f), slowDuration(2.0f) {
    // Slow tower uses farthest critter strategy
    setTargetingStrategy(std::make_unique<FarthestCritterStrategy>());
}

void SlowTower::upgrade() {
    Tower::upgrade();
    slowEffect *= 1.15f;
    slowDuration *= 1.1f;
}

void SlowTower::attackCritters(std::vector<Critter>& critters) {
    if (!canShoot()) return;
    
    Critter* target = targetingStrategy->selectTarget(critters, position, range);

    if (target) {
        // Create slow projectile
        TowerShot shot;
        shot.start = position;
        shot.end = target->getPosition();
        shot.currentPos = position;
        shot.speed = 700.0f; // Slower for slow tower
        shot.color = YELLOW;
        shot.timer = 0.6f;
        shot.hit = false;
        activeShots.push_back(shot);

        // Apply effects
        target->takeDamage(power);
        target->applySlowEffect(1.0f - slowEffect, slowDuration);
        
        resetShotTimer();
    }
}

// SniperTower implementation
SniperTower::SniperTower()
    : Tower(300.0f, 30.0f, 0.5f, 500, 100, "Sniper Tower", PURPLE),
      criticalChance(0.25f), criticalMultiplier(2.0f) {
    // Sniper tower uses lowest health critter strategy
    setTargetingStrategy(std::make_unique<LowestHealthStrategy>());
}

void SniperTower::upgrade() {
    Tower::upgrade();
    range *= 1.15f;  // Snipers get extra range on upgrade
    criticalChance *= 1.1f;
    if (criticalChance > 0.5f) criticalChance = 0.5f;  // Cap at 50%
}

void SniperTower::attackCritters(std::vector<Critter>& critters) {
    if (!canShoot()) return;
    
    // Use targeting strategy to select a target
    Critter* target = targetingStrategy->selectTarget(critters, position, range);

    if (target) {
        // Create slow projectile
        TowerShot shot;
        shot.start = position;
        shot.end = target->getPosition();
        shot.currentPos = position;
        shot.speed = 700.0f; // Slower for slow tower
        shot.color = PURPLE;
        shot.timer = 0.6f;
        shot.hit = false;
        activeShots.push_back(shot);
    }

    
    if (target) {
        // Calculate if critical hit occurs
        float finalDamage = power;
        bool isCritical = GetRandomValue(0, 100) < criticalChance * 100;
        
        if (isCritical) {
            finalDamage *= criticalMultiplier;
            // Visual effect for critical hit
            DrawText("CRITICAL!", target->getPosition().x, 
                    target->getPosition().y - 20, 15, RED);
        }
        
        target->takeDamage(finalDamage);
        
        // Draw sniper shot effect
        DrawLineEx(position, target->getPosition(), 2.0f, PURPLE);
        
        resetShotTimer();
    }
}
