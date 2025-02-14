#ifndef TOWER_H
#define TOWER_H

#include <iostream>
#include <vector>
#include "mapgen.h"  // Include the map class

using namespace std;

// Base Tower class
class Tower {
protected:
    int x, y;        // Tower position
    int buyCost;     // Cost to purchase the tower
    int refundValue; // Refund value when selling the tower
    int range;       // Attack range
    int power;       // Attack power
    int fireRate;    // Attack speed (shots per second)

public:
    Tower(int x, int y, int cost, int refund, int range, int power, int fireRate);
    virtual ~Tower() {}

    virtual void attack() = 0;  // Pure virtual function for attacking behavior

    int getX() { return x; }
    int getY() { return y; }
    int getRange() { return range; }
    int getPower() { return power; }
    int getBuyCost() { return buyCost; }
    int getRefundValue() { return refundValue; }
};

// BasicTower: Attacks a single target
class BasicTower : public Tower {
public:
    BasicTower(int x, int y);
    void attack() override;
};

// AoETower: Deals damage to multiple targets in range
class AoETower : public Tower {
public:
    AoETower(int x, int y);
    void attack() override;
};

// SlowTower: Slows down enemy movement
class SlowTower : public Tower {
public:
    SlowTower(int x, int y);
    void attack() override;
};

#endif // TOWER_H
