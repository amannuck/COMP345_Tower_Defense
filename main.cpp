#include "mapgen.h"

int main() {
    Map map(10, 6);
    map.generateRandomMap();
    map.display();

    if (map.validateMap()) {
        cout << "Map is valid!" << endl;
    } else {
        cout << "Map is invalid!" << endl;
    }

    return 0;
}
