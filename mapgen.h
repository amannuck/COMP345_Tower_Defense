#ifndef MAPGEN_H
#define MAPGEN_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/**
 * @enum CellType
 * @brief Defines the possible cell types in the game map.
 *
 * The map consists of two types of cells:
 * - SCENERY: Areas where towers can be placed, typically representing buildable terrain
 * - PATH: The route that critters (enemies) follow from entry to exit point
 */
enum CellType { SCENERY, PATH };

/**
 * @class Map
 * @brief Manages the game map generation and validation.
 *
 * This class is responsible for:
 * - Creating and managing the 2D grid that represents the game map
 * - Setting and validating path cells for critter movement
 * - Managing entry and exit points
 * - Ensuring map validity (connected path, proper entry/exit)
 * - Generating random valid maps
 *
 * The map uses a grid system where each cell is either SCENERY (for tower placement)
 * or PATH (for critter movement). A valid map must have:
 * - A continuous PATH from entry to exit point
 * - Exactly one entry and one exit point
 * - Sufficient SCENERY cells for tower placement
 */
class Map {
private:
    int width, height;                    // Dimensions of the map grid
    vector<vector<CellType>> grid;        // 2D grid representing the map layout
    pair<int, int> entryPoint;            // Starting point where critters spawn
    pair<int, int> exitPoint;             // End point where critters escape
    bool entrySet, exitSet;               // Flags to track if entry/exit points are defined

    /**
     * @brief Validates if given coordinates are within map boundaries
     * @param x X-coordinate to check
     * @param y Y-coordinate to check
     * @return true if coordinates are valid, false otherwise
     */
    bool isValidCoordinate(int x, int y) const;

    /**
     * @brief Checks if there exists a valid path from entry to exit point
     * Uses breadth-first search to verify path connectivity
     * @return true if a valid path exists, false otherwise
     */
    bool isPathConnected();

public:
    /**
     * @brief Constructs a new map with specified dimensions
     * @param w Width of the map
     * @param h Height of the map
     * Initializes all cells as SCENERY by default
     */
    Map(int w, int h);

    /**
     * @brief Marks a cell as part of the PATH
     * @param x X-coordinate of the cell
     * @param y Y-coordinate of the cell
     * Used for creating the route that critters will follow
     */
    void setPath(int x, int y);

    /**
     * @brief Sets the entry point for critters
     * @param x X-coordinate of entry point
     * @param y Y-coordinate of entry point
     * Must be placed on a PATH cell
     */
    void setEntry(int x, int y);

    /**
     * @brief Sets the exit point for critters
     * @param x X-coordinate of exit point
     * @param y Y-coordinate of exit point
     * Must be placed on a PATH cell
     */
    void setExit(int x, int y);

    /**
     * @brief Gets the current entry point coordinates
     * @return pair<int, int> containing (x,y) coordinates
     */
    pair<int, int> getEntry() const;

    /**
     * @brief Gets the current exit point coordinates
     * @return pair<int, int> containing (x,y) coordinates
     */
    pair<int, int> getExit() const;

    /**
     * @brief Displays the current map state to the console
     * Useful for debugging and development
     */
    void display();

    /**
     * @brief Validates the current map configuration
     * Checks for:
     * - Valid entry and exit points
     * - Connected path between entry and exit
     * - Sufficient buildable areas
     * @return true if map is valid, false otherwise
     */
    bool validateMap();

    /**
     * @brief Generates a random valid map layout
     * Creates a new path configuration with:
     * - Random entry and exit points
     * - Valid connected path between them
     * - Appropriate distribution of buildable areas
     */
    void generateRandomMap();

    /**
     * @brief Checks if a given cell is part of the PATH
     * @param x X-coordinate to check
     * @param y Y-coordinate to check
     * @return true if cell is PATH, false if SCENERY
     */
    bool isPath(int x, int y) const;
};

#endif // MAPGEN_H