#include "mapgen.h"
#include "critter.h"
#include "tower.h"
#include "CritterGroup.h"
#include "towerManager.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// Display map with both critters and towers
void displayMapWithCrittersAndTowers(const Map& map, const vector<Critter>& critters, const TowerManager& manager) {
    auto entry = map.getEntry();
    auto exit = map.getExit();

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            bool hasCritter = false;
            bool hasTower = false;

            // Check if a critter is at this position
            for (const auto& critter : critters) {
                if (critter.getPosition() == make_pair(x, y)) {
                    cout << "C ";
                    hasCritter = true;
                    break;
                }
            }

            // Check if a tower is at this position
            if (!hasCritter) {
                for (const auto& tower : manager.getTowers()) {
                    if (tower->getX() == x && tower->getY() == y) {
                        cout << "T ";
                        hasTower = true;
                        break;
                    }
                }
            }

            // Display map elements
            if (!hasCritter && !hasTower) {
                if (make_pair(x, y) == entry) {
                    cout << "E ";
                } else if (make_pair(x, y) == exit) {
                    cout << "X ";
                } else if (map.isPath(x, y)) {
                    cout << "# ";
                } else {
                    cout << ". ";
                }
            }
        }
        cout << endl;
    }
    cout << "\nLegend: C = Critter, T = Tower, E = Entry, X = Exit, # = Path, . = Scenery\n";
}

int main() {
    // Create a 10x10 map
    Map map(10, 10);
    map.generateRandomMap();

    // Create TowerManager with 500 starting gold
    TowerManager manager(&map, 500);

    // Place towers dynamically using TowerManager
    manager.buyTower(2, 3, "Basic");
    manager.buyTower(5, 5, "AoE");
    manager.buyTower(8, 6, "Slow");

    cout << "Initial map state with towers:" << endl;
    displayMapWithCrittersAndTowers(map, {}, manager);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Create critter group
    CritterGroup group(&map);

    // Generate first wave
    int numCritters = group.generateWave();
    cout << "Generated wave with " << numCritters << " critters" << endl;

    // Main simulation loop
    while (!group.isWaveComplete()) {
        // Try to spawn a new critter
        group.spawnNextCritter();

        // Get current critters
        auto& critters = group.getActiveCritters();

        // Display current state
        displayMapWithCrittersAndTowers(map, critters, manager);

        // Towers attack critters
        manager.updateTowers(critters);

        // Move all critters
        group.moveAllCritters([](int damage) {
            cout << "A critter reached the exit! Damage dealt: " << damage << endl;
        });

        // Wait a bit before next update
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Display stats
        cout << "Active critters: " << critters.size()
             << ", Remaining spawns: " << group.getRemainingSpawns() << endl;
    }

    cout << "Wave complete!" << endl;
    return 0;
}
