#include "mapgen.h"
#include "critter.h"
#include "tower.h"
#include "CritterGroup.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

/**
 * @brief Displays the game map with critters and towers.
 *
 * This function prints a **10x10 grid**, displaying:
 * - `C` for Critters
 * - `T` for Towers
 * - `E` for Entry
 * - `X` for Exit
 * - `#` for Path
 * - `.` for Empty Land
 */
void displayGameState(const Map& map, const vector<Critter>& critters, const vector<Tower*>& towers) {
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
                for (const auto& tower : towers) {
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
    // Create a 10x10 game map
    Map map(10, 10);
    map.generateRandomMap();

    // Create towers and register them on the map
    vector<Tower*> towers;

    if (map.placeTower(2, 3)) towers.push_back(new BasicTower(2, 3));
    if (map.placeTower(5, 5)) towers.push_back(new AoETower(5, 5));
    if (map.placeTower(8, 6)) towers.push_back(new SlowTower(8, 6));

    // Display initial map state with towers
    cout << "Initial map state with towers:" << endl;
    displayGameState(map, {}, towers);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Create critter group (enemies)
    CritterGroup group(&map);

    // Generate the first wave of critters
    int numCritters = group.generateWave();
    cout << "Generated wave with " << numCritters << " critters" << endl;

    // Main game simulation loop
    while (!group.isWaveComplete()) {
        // Try to spawn a new critter
        group.spawnNextCritter();

        // Get all active critters
        auto& critters = group.getActiveCritters();

        // Display current game state
        displayGameState(map, critters, towers);

        // Towers attack critters
        for (Tower* tower : towers) {
            tower->attack(critters);
        }

        // Move all critters toward the exit
        group.moveAllCritters([](int damage) {
            cout << "A critter reached the exit! Player takes " << damage << " damage!\n";
        });

        // Wait before the next update (controls game speed)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Display wave status
        cout << "Active critters: " << critters.size()
             << ", Remaining spawns: " << group.getRemainingSpawns() << endl;
    }

    cout << "\nWave complete! All critters are gone!" << endl;

    // Free memory (since towers were created dynamically)
    for (Tower* tower : towers) {
        delete tower;
    }

    return 0;
}
