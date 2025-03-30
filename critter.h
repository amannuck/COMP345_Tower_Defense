// Critter.h
#pragma once
#include <vector>
#include <string>
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
    std::string type;  // New: Type of critter (Basic, Fast, Tank, Boss)
    
    // Slow effect variables
    float slowFactor = 1.0f;
    float slowDuration = 0.0f;

    std::vector<CritterObserver*> observers;

public:
    // Updated constructor to include type
    Critter(int level, float speed, float hp, int reward, int strength, 
            const std::vector<Vector2>& path, const std::string& type);

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
    const std::string& getType() const { return type; }  // New: Get critter type
    
    // Slow effect method
    void applySlowEffect(float factor, float duration);

    // Observer management
    void addObserver(CritterObserver* observer);
    void removeObserver(CritterObserver* observer);
    void notifyReachedEnd();
    void notifyDefeated();
};