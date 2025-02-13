#ifndef MAPGEN_H
#define MAPGEN_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Defines the possible cell types in the map
enum CellType { SCENERY, PATH };  // SCENERY is where towers can be placed, PATH is where critters can move

class Map {
private:
    int width, height;                    // Dimensions of the map
    vector<vector<CellType>> grid;        // 2D grid representing the map
    pair<int, int> entryPoint;            // Starting point for critters
    pair<int, int> exitPoint;             // End point for critters
    bool entrySet, exitSet;               // Flags to track if entry/exit are set

    // Helper functions
    bool isValidCoordinate(int x, int y) const; // Checks if coordinates are within map bounds
    bool isPathConnected();               // Checks if there's a valid path from entry to exit

public:
    // Constructor and member functions
    Map(int w, int h);                          // Creates a map of given width and height
    void setPath(int x, int y);                 // Marks a cell as PATH
    void setEntry(int x, int y);                // Sets the entry point
    void setExit(int x, int y);                 // Sets the exit point
    pair<int, int> getEntry() const;             // Returns the exit point
    pair<int, int> getExit() const;             // Returns the exit point
    void display();                             // Prints the map to console
    bool validateMap();                         // Checks if the map is valid
    void generateRandomMap();                   // Creates a random valid map
    bool isPath(int x, int y) const;            // Checks if a cell is a PATH cell
};

#endif // MAPGEN_H
