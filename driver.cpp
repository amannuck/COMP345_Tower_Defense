#include "mapgen.h"

int main() {
    Map map(10, 6); // width=10, height=6
    map.generateRandomMap(); // generate random map
    map.display(); // display the map

    if (map.validateMap()) {
        cout << "Map is valid!" << endl;
    } else {
        cout << "Map is invalid!" << endl;
    }

    return 0;
}
