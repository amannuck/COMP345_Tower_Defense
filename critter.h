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
    void setActive(bool state) { active = state; }
    int getReward() const { return reward; }
    int getStrength() const { return strength; }

    // Observer management
    void addObserver(CritterObserver* observer);
    void removeObserver(CritterObserver* observer);
    void notifyReachedEnd();
    void notifyDefeated();
};