#pragma once
#include <vector>
#include "raylib.h"
#include "CritterObserver.h"

class Critter {
private:
    int level;
    float speed;
    float hitPoints, maxHitPoints;
    int reward, strength;
    Vector2 position;
    std::vector<Vector2> path;
    int pathIndex;
    bool active;
    bool reachedEndFlag = false;
    
    // Add slow effect variables
    float slowFactor = 1.0f;  // 1.0 = normal speed, less than 1 = slowed
    float slowDuration = 0.0f;  // Duration of slow effect

    std::vector<CritterObserver*> observers;  // List of observers

public:
    Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path);

    void setPosition(const Vector2 &gridPos, int cellSize, int offsetX, int offsetY);

    void move();
    void takeDamage(float damage);
    bool isDead() const;
    bool reachedEnd() const;
    void draw() const;
    void activate();
    bool isActive() const { return active; }

    void setPosition(const Vector2& newPos) { position = newPos; }
    const Vector2 getPosition() const { return position; }
    void setActive(bool state) { active = state; }
    int getReward() const { return reward; }
    int getStrength() const { return strength; }
    float getHitPoints() const { return hitPoints; }
    int getPathIndex() const { return pathIndex; }
    
    // Add slow effect method
    void applySlowEffect(float factor, float duration);

    // Observer management
    void addObserver(CritterObserver* observer);
    void removeObserver(CritterObserver* observer);
    void notifyReachedEnd();
    void notifyDefeated();
};
