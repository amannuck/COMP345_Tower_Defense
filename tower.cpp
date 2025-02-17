/**
 * @file tower.cpp
 * @brief Implementation of the Tower class for the Tower Defense game.
 */

#include "tower.h"

/**
 * @brief Constructs a Tower object with specified properties.
 *
 * @param x X-coordinate of the tower.
 * @param y Y-coordinate of the tower.
 * @param cost Initial cost of the tower.
 * @param refund Refund value when selling the tower.
 * @param range Attack range of the tower.
 * @param power Attack power of the tower.
 * @param fireRate Rate of fire of the tower.
 * @param upgradeCost Cost to upgrade the tower.
 */
Tower::Tower(int x, int y, int cost, int refund, int range, int power, int fireRate, int upgradeCost)
        : x(x), y(y), buyCost(cost), refundValue(refund), range(range), power(power), fireRate(fireRate), upgradeCost(upgradeCost), level(1) {}

/**
 * @brief Upgrades the tower by increasing attack power and refund value.
 *
 * The tower can be upgraded up to a maximum level of 3.
 */
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

/**
 * @brief Constructs a BasicTower with predefined attributes.
 *
 * @param x X-coordinate of the tower.
 * @param y Y-coordinate of the tower.
 */
BasicTower::BasicTower(int x, int y) : Tower(x, y, 100, 50, 3, 10, 1, 50) {}

/**
 * @brief Attacks the first critter within range.
 *
 * @param critters List of active critters on the map.
 */
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

/**
 * @brief Constructs an AoETower with predefined attributes.
 *
 * @param x X-coordinate of the tower.
 * @param y Y-coordinate of the tower.
 */
AoETower::AoETower(int x, int y) : Tower(x, y, 200, 100, 2, 7, 1, 75) {}

/**
 * @brief Attacks multiple critters within range.
 *
 * @param critters List of active critters on the map.
 */
void AoETower::attack(vector<Critter>& critters) {
    for (Critter& critter : critters) {
        if (critter.isDead()) continue; // Skip dead enemies
        if (abs(critter.getPosition().first - x) + abs(critter.getPosition().second - y) <= range) {
            critter.takeDamage(power);  // Apply area damage
            cout << "AoETower at (" << x << ", " << y << ") hit multiple critters for " << power << " damage!\n";
        }
    }
}