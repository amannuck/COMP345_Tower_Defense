#ifndef TOWER_H
#define TOWER_H

#include <iostream>
#include <vector>
#include "mapgen.h"
#include "critter.h"

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
    int level;       // Tower level (1, 2, 3)
    int upgradeCost; // Cost to upgrade

public:
    Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost);
    virtual ~Tower() {}

    virtual void attack(vector<Critter>& critters) = 0; // Attack method

    void upgrade();    // Upgrade tower level
    int getX() { return x; }
    int getY() { return y; }
    int getRange() { return range; }
    int getPower() { return power; }
    int getBuyCost() { return buyCost; }
    int getRefundValue() { return refundValue; }
    int getLevel() { return level; }
};

// BasicTower: Single target attack
class BasicTower : public Tower {
public:
    BasicTower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

// AoETower: Area of Effect attack
class AoETower : public Tower {
public:
    AoETower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

// SlowTower: Slows enemies instead of dealing damage
class SlowTower : public Tower {
public:
    SlowTower(int x, int y);
    void attack(vector<Critter>& critters) override;
};

// Tower Manager (handles buying, selling, and upgrading towers)
class TowerManager {
private:
    vector<Tower*> towers;
    int playerGold;
    Map* map;

public:
    TowerManager(Map* gameMap, int initialGold);
    ~TowerManager();

    bool buyTower(int x, int y, string type);
    bool sellTower(int x, int y);
    bool upgradeTower(int x, int y);
    void updateTowers(vector<Critter>& critters);
    void displayTowers();
};

#endif // TOWER_H

