#include "mapgen.h"
#include "critter.h"
#include "tower.h"
#include "CritterGroup.h"
#include <iostream>
#include <chrono>
#include <thread>

// Test function to display map with critters
void displayMapWithCritters(const Map& map, const vector<Critter>& critters) {
    // Clear screen
    // cout << "\033[2J\033[1;1H";

    // Get map entry and exit points
    auto entry = map.getEntry();
    auto exit = map.getExit();

    // Display the map with critters
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            bool hasCritter = false;
            // Check if there's a critter at this position
            for (const auto& critter : critters) {
                if (critter.getPosition() == make_pair(x, y)) {
                    cout << "C ";
                    hasCritter = true;
                    break;
                }
            }
            if (!hasCritter) {
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
    cout << "\nLegend: C = Critter, E = Entry, X = Exit, # = Path, . = Scenery\n";
}

int main() {
    // Create a 10x10 map
    Map map(10, 10);
    map.generateRandomMap();

    // Placing towers on the map
    cout << "Placing Towers...\n";
    BasicTower t1(2, 3);
    AoETower t2(5, 5);
    SlowTower t3(8, 6);

    // Attack test
    t1.attack();
    t2.attack();
    t3.attack();

    // Create critter group
    CritterGroup group(&map);

    cout << "Initial map state:" << endl;
    map.display();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Generate first wave
    int numCritters = group.generateWave();
    cout << "Generated wave with " << numCritters << " critters" << endl;

    // Main simulation loop
    while (!group.isWaveComplete()) {
        // Try to spawn a new critter
        group.spawnNextCritter();

        // Get current critters
        const auto& critters = group.getActiveCritters();

        // Display current state
        displayMapWithCritters(map, critters);

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