/**
 * @file Tower.cpp
 * @brief Implementation of the Tower class and its derived tower types
 * @details This file provides the implementation for the base Tower class as well as
 *          specialized tower types (Basic, Area, Slow, and Sniper). It also includes
 *          functions for managing tower shots and visual effects.
 */

#include "Tower.h"
#include <cmath>
#include "raymath.h"

/**
 * @brief Constructor for the Tower class
 * @param range Maximum attack range of the tower
 * @param power Damage dealt per attack
 * @param fireRate Attacks per second
 * @param buyCost Initial purchase cost in currency
 * @param upgradeCost Cost to upgrade the tower
 * @param name Display name of the tower
 * @param color Color used to render the tower
 * @details Initializes a tower with the specified attributes and sets the
 *          default targeting strategy to FirstInRangeStrategy.
 */
Tower::Tower(float range, float power, float fireRate, int buyCost, int upgradeCost,
             const std::string& name, const Color& color)
    : level(1), range(range), power(power), fireRate(fireRate), lastShotTime(0),
      position({0, 0}), buyCost(buyCost), upgradeCost(upgradeCost), name(name), color(color) {
    // Default to FirstInRangeStrategy
    targetingStrategy = std::make_unique<FirstInRangeStrategy>();
}

/**
 * @brief Destructor for the Tower class
 */
Tower::~Tower() {}

/**
 * @brief Upgrades the tower's capabilities
 * @details Increases the tower's level and improves its range, power, and fire rate.
 *          Also increases the cost of the next upgrade.
 */
void Tower::upgrade() {
    level++;
    range *= 1.2f;
    power *= 1.3f;
    fireRate *= 1.1f;
    upgradeCost = static_cast<int>(upgradeCost * 1.5f);
}

/**
 * @brief Calculates the refund value when selling the tower
 * @return The amount of currency refunded
 * @details Returns 70% of the tower's purchase cost.
 */
int Tower::getRefundValue() const {
    float totalCost = buyCost;
    float refundRatio = 0.7f;  // 70% refund
    return static_cast<int>(totalCost * refundRatio);
}

// Add this static member to track active shots
static std::vector<TowerShot> activeShots;

/**
 * @brief Updates all active tower shots
 * @param deltaTime Time elapsed since the last update
 * @details Moves projectiles toward their targets, detects hits, and removes
 *          expired shots based on their timers.
 */
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

/**
 * @brief Renders all active tower shots
 * @details Draws projectiles as animated triangles pointing toward their targets,
 *          with optional trails and fade-out effects based on their timers.
 */
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

/**
 * @brief Attacks nearby critters based on the tower's targeting strategy
 * @param critters Vector of critters to target
 * @details Selects a target using the tower's targeting strategy, creates
 *          a visual projectile, and applies damage to the target.
 */
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

/**
 * @brief Renders the tower
 * @details Draws the tower as a colored circle with a level indicator.
 */
void Tower::draw() const {
    // Draw tower
    DrawCircle(position.x, position.y, 15, color);

    // Draw level indicator
    std::string levelText = "Lvl " + std::to_string(level);
    DrawText(levelText.c_str(), position.x - 10, position.y - 25, 10, BLACK);

    // If you want to draw shots here instead of in Game.cpp
    DrawTowerShots();
}

/**
 * @brief Checks if the tower can shoot
 * @return true if enough time has passed since the last shot, false otherwise
 * @details Uses the tower's fire rate to determine if it's ready to attack again.
 */
bool Tower::canShoot() const {
    return GetTime() - lastShotTime >= 1.0f / fireRate;
}

/**
 * @brief Resets the shot timer after an attack
 * @details Updates the last shot time to the current time.
 */
void Tower::resetShotTimer() {
    lastShotTime = GetTime();
}

/**
 * @brief Constructor for the BasicTower class
 * @details Initializes a basic tower with balanced stats and the
 *          NearestCritterStrategy targeting strategy.
 */
BasicTower::BasicTower()
    : Tower(150.0f, 10.0f, 1.0f, 100, 50, "Basic Tower", RED) {
    // Basic tower uses nearest critter strategy
    setTargetingStrategy(std::make_unique<NearestCritterStrategy>());
}

/**
 * @brief Upgrades the BasicTower
 * @details Calls the base class upgrade method and applies an additional
 *          power multiplier specific to BasicTower.
 */
void BasicTower::upgrade() {
    Tower::upgrade();
    // Basic tower gets extra power on upgrade
    power *= 1.1f;
}

/**
 * @brief Constructor for the AreaTower class
 * @details Initializes an area effect tower with the FirstInRangeStrategy
 *          targeting strategy and area damage capabilities.
 */
AreaTower::AreaTower()
    : Tower(120.0f, 8.0f, 0.8f, 150, 75, "Area Tower", BLUE), areaRadius(40.0f) {
    // Area tower uses first in range strategy
    setTargetingStrategy(std::make_unique<FirstInRangeStrategy>());
}

/**
 * @brief Upgrades the AreaTower
 * @details Calls the base class upgrade method and increases the area of effect.
 */
void AreaTower::upgrade() {
    Tower::upgrade();
    areaRadius *= 1.2f;
}

/**
 * @brief Area tower's specialized attack method
 * @param critters Vector of critters to target
 * @details Selects a main target and damages all critters within the area radius
 *          of that target. Creates visual effects for the area attack.
 */
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

/**
 * @brief Constructor for the SlowTower class
 * @details Initializes a tower that slows enemies using the FarthestCritterStrategy.
 */
SlowTower::SlowTower()
    : Tower(130.0f, 5.0f, 1.2f, 125, 60, "Slow Tower", YELLOW),
      slowEffect(0.3f), slowDuration(2.0f) {
    // Slow tower uses farthest critter strategy
    setTargetingStrategy(std::make_unique<FarthestCritterStrategy>());
}

/**
 * @brief Upgrades the SlowTower
 * @details Calls the base class upgrade method and increases slow effect and duration.
 */
void SlowTower::upgrade() {
    Tower::upgrade();
    slowEffect *= 1.15f;
    slowDuration *= 1.1f;
}

/**
 * @brief Slow tower's specialized attack method
 * @param critters Vector of critters to target
 * @details Selects a target, applies damage, and applies a slow effect to the target.
 */
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

/**
 * @brief Constructor for the SniperTower class
 * @details Initializes a high-damage, long-range tower with the LowestHealthStrategy
 *          and critical hit capabilities.
 */
SniperTower::SniperTower()
    : Tower(300.0f, 30.0f, 0.5f, 500, 100, "Sniper Tower", PURPLE),
      criticalChance(0.25f), criticalMultiplier(2.0f) {
    // Sniper tower uses lowest health critter strategy
    setTargetingStrategy(std::make_unique<LowestHealthStrategy>());
}

/**
 * @brief Upgrades the SniperTower
 * @details Calls the base class upgrade method and increases range and critical hit chance.
 */
void SniperTower::upgrade() {
    Tower::upgrade();
    range *= 1.15f;  // Snipers get extra range on upgrade
    criticalChance *= 1.1f;
    if (criticalChance > 0.5f) criticalChance = 0.5f;  // Cap at 50%
}

/**
 * @brief Sniper tower's specialized attack method
 * @param critters Vector of critters to target
 * @details Selects a target and applies damage with a chance for critical hits
 *          that deal increased damage. Displays visual effects for the sniper shot.
 */
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
