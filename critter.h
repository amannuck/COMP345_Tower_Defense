//#pragma once
//#include <raylib.h>
//#include <vector>
//
//class Critter {
//private:
//    int level;
//    float speed;
//    float hitPoints;
//    float maxHitPoints;
//    int reward;
//    int strength;
//    Vector2 position;
//    std::vector<Vector2> path; // Movement path
//    int pathIndex; // Current position in path
//    bool active = false;
//    int currentPathIndex = 0; // Tracks movement along path
//    bool reachedEndFlag = false; // Tracks if critter reached exit
//
//public:
//    Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path);
//    ~Critter() = default;
//
//    void move();
//    void takeDamage(float damage);
//    bool isDead() const;
//    bool reachedEnd() const;
//    int getReward() const { return reward; }
//    int getStrength() const { return strength; }
//    void draw() const;
//    void setActive(bool value) { active = value; }
//    bool isActive() const { return active; }
//    void setPosition(Vector2 newPos) { position = newPos; }
//    Vector2 getPosition() const { return position; }
//    void activate();
//};

#pragma once
#include <vector>
#include "raylib.h"

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

public:
    Critter(int level, float speed, float hp, int reward, int strength, const std::vector<Vector2>& path);

    void move();
    void takeDamage(float damage);
    bool isDead() const;
    bool reachedEnd() const;
    void draw(int offsetX = 0, int offsetY = 0, int cellSize = 1) const;
    void activate();
    bool isActive() const { return active; }

    // ✅ Fix: Add these missing methods
    void setPosition(Vector2 newPos) { position = newPos; }
    void setActive(bool state) { active = state; }
    int getReward() const { return reward; }
    int getStrength() const { return strength; }
};
