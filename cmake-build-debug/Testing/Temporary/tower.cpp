#include "tower.h"

// Base Tower constructor
Tower::Tower(int x, int y, int cost, int refund, int range, int power, int fireRate)
    : x(x), y(y), buyCost(cost), refundValue(refund), range(range), power(power), fireRate(fireRate) {}

// BasicTower: Attacks a single target
BasicTower::BasicTower(int x, int y) : Tower(x, y, 100, 50, 3, 10, 1) {}

void BasicTower::attack() {
    cout << "BasicTower at (" << x << ", " << y << ") attacks a single target with " << power << " damage.\n";
}

// AoETower: Attacks all targets in range
AoETower::AoETower(int x, int y) : Tower(x, y, 200, 100, 2, 7, 1) {}

void AoETower::attack() {
    cout << "AoETower at (" << x << ", " << y << ") attacks all targets within range with " << power << " damage.\n";
}

// SlowTower: Slows down the target instead of dealing direct damage
SlowTower::SlowTower(int x, int y) : Tower(x, y, 150, 75, 4, 5, 1) {}

void SlowTower::attack() {
    cout << "SlowTower at (" << x << ", " << y << ") attacks a target and slows it down.\n";
}