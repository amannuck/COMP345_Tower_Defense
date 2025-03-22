// TowerDecorators.cpp
#include "Tower.h"

// SplashDecorator implementation
void SplashDecorator::attackCritters(std::vector<Critter>& critters) {
    if (!wrappedTower->canShoot()) return;
    
    // Use the base attackCritters to find and hit the primary target
    Critter* primaryTarget = nullptr;
    
    // First, find a target
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(getPosition(), critter.getPosition());
            if (distance <= getRange()) {
                primaryTarget = &critter;
                break;
            }
        }
    }
    
    if (primaryTarget) {
        // The tower found a target, so apply damage to it
        primaryTarget->takeDamage(getPower());
        
        // Now apply splash damage to nearby critters
        Vector2 impactPoint = primaryTarget->getPosition();
        
        for (auto& critter : critters) {
            if (&critter != primaryTarget && critter.isActive() && !critter.isDead()) {
                float distance = Vector2Distance(impactPoint, critter.getPosition());
                if (distance <= splashRadius) {
                    // Apply partial damage to critters in splash radius
                    float splashDamage = getPower() * splashDamagePercent;
                    critter.takeDamage(splashDamage);
                    
                    // Visual effect for splash damage
                    DrawLineEx(impactPoint, critter.getPosition(), 1.0f, 
                               ColorAlpha(ORANGE, 0.6f));
                }
            }
        }
        
        // Draw splash area
        DrawCircleV(impactPoint, splashRadius, ColorAlpha(ORANGE, 0.2f));
        
        // Reset shot timer
        wrappedTower->resetShotTimer();
    }
    else {
        // No primary target found, delegate to wrapped tower
        wrappedTower->attackCritters(critters);
    }
}

// BurningDecorator implementation
void BurningDecorator::attackCritters(std::vector<Critter>& critters) {
    // Process burning damage on already burning critters
    float deltaTime = GetFrameTime();
    
    // Remove burned-out critters and apply damage to still-burning ones
    burningCritters.erase(
        std::remove_if(burningCritters.begin(), burningCritters.end(),
            [deltaTime, this](auto& pair) {
                Critter* critter = pair.first;
                float& remainingTime = pair.second;
                
                // Skip if critter is already dead or no longer active
                if (critter->isDead() || !critter->isActive()) return true;
                
                // Apply burn damage
                remainingTime -= deltaTime;
                float burnDamage = burnDamagePerSecond * deltaTime;
                critter->takeDamage(burnDamage);
                
                // Draw burning visual effect
                DrawCircleV(critter->getPosition(), 12, ColorAlpha(RED, 0.3f));
                
                // Return true to remove if burn duration expired
                return remainingTime <= 0;
            }),
        burningCritters.end()
    );
    
    // Let the wrapped tower attack normally
    wrappedTower->attackCritters(critters);
    
    // Check if the tower found a target to apply burning effect to
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(getPosition(), critter.getPosition());
            if (distance <= getRange()) {
                // Check if this critter is already burning
                auto it = std::find_if(burningCritters.begin(), burningCritters.end(),
                    [&critter](const auto& pair) { return pair.first == &critter; });
                
                if (it == burningCritters.end()) {
                    // Not found in burning critters, so add it
                    burningCritters.emplace_back(&critter, burnDuration);
                } else {
                    // Already burning, reset the duration
                    it->second = burnDuration;
                }
                
                // We only want to apply burning to one critter per shot
                break;
            }
        }
    }
}

// FreezingDecorator implementation
void FreezingDecorator::attackCritters(std::vector<Critter>& critters) {
    // Use the base attackCritters to find and hit the primary target
    Critter* primaryTarget = nullptr;
    
    // First, find a target
    for (auto& critter : critters) {
        if (critter.isActive() && !critter.isDead()) {
            float distance = Vector2Distance(getPosition(), critter.getPosition());
            if (distance <= getRange()) {
                primaryTarget = &critter;
                break;
            }
        }
    }
    
    if (primaryTarget) {
        // Apply base damage
        primaryTarget->takeDamage(getPower());
        
        // Apply freezing effect
        primaryTarget->applySlowEffect(1.0f - freezeEffect, freezeDuration);
        
        // Visual effect for freeze attack
        DrawLineEx(getPosition(), primaryTarget->getPosition(), 2.0f, SKYBLUE);
        DrawCircleV(primaryTarget->getPosition(), 15, ColorAlpha(SKYBLUE, 0.3f));
        
        // Reset shot timer
        wrappedTower->resetShotTimer();
    }
    else {
        // No primary target found, delegate to wrapped tower
        wrappedTower->attackCritters(critters);
    }
}
