/**
 * @file tower.cpp
 * @brief Implementation of the Tower class for the Tower Defense game.
 */

#include "tower.h"

/**
 * @brief Constructs a Tower object with specified properties.
 */
Tower::Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost)
    : x(x), y(y), buyCost(cost), refundValue(refund), range(range), power(power), fireRate(fireRate), upgradeCost(upgradeCost), level(1) {
    cout << "Tower created at (" << x << ", " << y << ")\n";
}

/**
 * @brief Upgrades the tower, increasing attack power and refund value.
 */
void Tower::upgrade() {
    if (level < 3) {
        level++;
        power += 5;
        refundValue += 25;
        cout << "Tower at (" << x << ", " << y << ") upgraded to level " << level << "!\n";
    } else {
        cout << "Tower is already at max level!\n";
    }
}

/**
 * @brief Constructs a BasicTower with predefined attributes.
 */
BasicTower::BasicTower(int x, int y) : Tower(x, y, 100, 50, 3, 10, 1, 50) {}

/**
 * @brief Attacks the first critter within range.
 */
void BasicTower::attack(vector<Critter>& critters) {
    for (Critter& critter : critters) {
        if (critter.isDead()) continue;
        if (abs(critter.getPosition().first - x) + abs(critter.getPosition().second - y) <= range) {
            critter.takeDamage(power);
            cout << "BasicTower at (" << x << ", " << y << ") hit a critter for " << power << " damage!\n";
            return;
        }
    }
}

/**
 * @brief Constructs an AoETower with predefined attributes.
 */
AoETower::AoETower(int x, int y) : Tower(x, y, 200, 100, 2, 7, 1, 75) {}

/**
 * @brief Attacks multiple critters within range.
 */
void AoETower::attack(vector<Critter>& critters) {
    for (Critter& critter : critters) {
        if (critter.isDead()) continue;
        if (abs(critter.getPosition().first - x) + abs(critter.getPosition().second - y) <= range) {
            critter.takeDamage(power);
            cout << "AoETower at (" << x << ", " << y << ") hit multiple critters for " << power << " damage!\n";
        }
    }
}

/**
 * @brief Allows the user to place a tower interactively.
 */
void placeTowerInteractive(Map& map, vector<Tower*>& towers) {
    int x, y;
    cout << "Enter tower coordinates (x y): ";
    cin >> x >> y;

    if (!map.isValidCoordinate(x, y)) {
        cout << "Invalid coordinates!\n";
        return;
    }

    if (map.isPath(x, y)) {
        cout << "Cannot place a tower on a path!\n";
        return;
    }

    for (Tower* t : towers) {
        if (t->getX() == x && t->getY() == y) {
            cout << "There is already a tower here!\n";
            return;
        }
    }

    cout << "Choose a tower type:\n1. Basic Tower (100 gold)\n2. AoE Tower (200 gold)\nEnter choice: ";
    int choice;
    cin >> choice;

    Tower* newTower = nullptr;
    if (choice == 1) {
        newTower = new BasicTower(x, y);
    } else if (choice == 2) {
        newTower = new AoETower(x, y);
    } else {
        cout << "Invalid choice!\n";
        return;
    }

    towers.push_back(newTower);
    cout << "Tower placed at (" << x << ", " << y << ")\n";
}
