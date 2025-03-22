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

void Tower::attackCritters(std::vector<Critter>& critters) {
    if (!canShoot()) return;  // Only attack if the tower can shoot

    // Use targeting strategy to select a target
    Critter* target = targetingStrategy->selectTarget(critters, position, range);
    
    if (target) {
        // Attack the target
        target->takeDamage(power);
        
        // Draw attack effect
        DrawLineEx(position, target->getPosition(), 2.0f, color);
        
        resetShotTimer();  // Reset the shot timer after attacking
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
    
    // Use targeting strategy to select main target
    Critter* mainTarget = targetingStrategy->selectTarget(critters, position, range);
    
    if (mainTarget) {
        Vector2 impactPoint = mainTarget->getPosition();
        int hitCount = 0;
        
        // Damage all critters in the area
        for (auto& critter : critters) {
            if (critter.isActive() && !critter.isDead()) {
                float distance = Vector2Distance(impactPoint, critter.getPosition());
                if (distance <= areaRadius) {
                    critter.takeDamage(power);
                    hitCount++;
                    
                    // Draw attack effect
                    DrawLineEx(position, critter.getPosition(), 1.0f, ColorAlpha(BLUE, 0.6f));
                }
            }
        }
        
        // Only reset shot timer if at least one critter was hit
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
    
    // Use targeting strategy to select a target
    Critter* target = targetingStrategy->selectTarget(critters, position, range);
    
    if (target) {
        // Apply damage
        target->takeDamage(power);
        
        // Apply slow effect - ensure this method is implemented in Critter class
        target->applySlowEffect(1.0f - slowEffect, slowDuration);
        
        // Draw slow attack effect
        DrawLineEx(position, target->getPosition(), 2.0f, YELLOW);
        DrawCircleV(target->getPosition(), 15, ColorAlpha(SKYBLUE, 0.3f));
        
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
