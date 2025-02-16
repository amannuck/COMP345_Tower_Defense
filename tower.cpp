#include "tower.h"

// **Base Tower Constructor**
// Initializes the tower with position, attack power, cost, and range.
Tower::Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost)
    : x(x), y(y), buyCost(cost), refundValue(refund), range(range), power(power), fireRate(fireRate), upgradeCost(upgradeCost), level(1) {}

// **Upgrade Tower**
// Increases attack power and refund value. Max level is 3.
void Tower::upgrade() {
    if (level < 3) {
        level++;
        power += 5;  // Increase attack power
        refundValue += 25; // Increase refund value
        cout << "Tower at (" << x << ", " << y << ") upgraded to level " << level << "!\n";
    } else {
        cout << "Tower is already at max level!\n";
    }
}

// **BasicTower: Single-target attack**
BasicTower::BasicTower(int x, int y) : Tower(x, y, 100, 50, 3, 10, 1, 50) {}

void BasicTower::attack(vector<Critter>& critters) {
    for (Critter& critter : critters) {
        if (critter.isDead()) continue;
        if (abs(critter.getPosition().first - x) + abs(critter.getPosition().second - y) <= range) {
            critter.takeDamage(power);  // Deal damage
            cout << "BasicTower at (" << x << ", " << y << ") hit a critter for " << power << " damage!\n";
            return;  // Only attacks one target
        }
    }
}

// **AoETower: Area attack**
AoETower::AoETower(int x, int y) : Tower(x, y, 200, 100, 2, 7, 1, 75) {}

void AoETower::attack(vector<Critter>& critters) {
    for (Critter& critter : critters) {
        if (critter.isDead()) continue; // Skip dead enemies
        if (abs(critter.getPosition().first - x) + abs(critter.getPosition().second - y) <= range) {
            critter.takeDamage(power);  // Apply area damage
            cout << "AoETower at (" << x << ", " << y << ") hit multiple critters for " << power << " damage!\n";
        }
    }
}

// **SlowTower: Slowing effect**
SlowTower::SlowTower(int x, int y) : Tower(x, y, 150, 75, 4, 5, 1, 60) {}

void SlowTower::attack(vector<Critter>& critters) {
    for (Critter& critter : critters) {
        if (critter.isDead()) continue;
        if (abs(critter.getPosition().first - x) + abs(critter.getPosition().second - y) <= range) {
            // Simulating slowing effect (Reduce critter speed)
            int newSpeed = max(critter.getSpeed() - 1, 1);
            critter.setSpeed(newSpeed); // Apply the slow effect
            cout << "SlowTower at (" << x << ", " << y << ") slowed a critter! New speed: " << newSpeed << "\n";
        }
    }
}
