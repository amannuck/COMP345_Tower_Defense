#include <iostream>
#include "tower.h"
#include "mapgen.h"

int main() {
    // Create a game map
    Map gameMap(10, 10);
    gameMap.generateRandomMap();
    gameMap.display();

    // Placing towers on the map
    cout << "Placing Towers...\n";
    BasicTower t1(2, 3);
    AoETower t2(5, 5);
    SlowTower t3(8, 6);

    // Attack test
    t1.attack();
    t2.attack();
    t3.attack();

    return 0;
}
