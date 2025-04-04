/**
 * @file Map.h
 * @brief Defines the Map class for managing the game grid
 * @details This class handles map creation, path finding, tower placement,
 *          and rendering of the game map for the tower defense game.
 */

#pragma once
#include <vector>
#include <queue>
#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include "IObserver.h"

/**
 * @enum CellType
 * @brief Defines the possible types of cells in the map grid
 */
enum class CellType {
    PATH,    ///< Path for critters to follow
    SCENERY, ///< Background terrain (can place towers here)
    ENTRY,   ///< Starting point for critters
    EXIT,    ///< Ending point for critters
    TOWER    ///< Location where a tower has been placed
};

/**
 * @class Map
 * @brief Manages the game map grid and pathfinding
 * @details Implements the IObservable interface to notify observers
 *          when the map state changes (e.g., tower placement, path updates)
 */
class Map : public IObservable {
private:
    int width;                                ///< Width of the map in cells
    int height;                               ///< Height of the map in cells
    std::vector<std::vector<CellType>> grid;  ///< 2D grid of cell types
    Vector2 entryPoint;                       ///< Coordinates of the entry point
    Vector2 exitPoint;                        ///< Coordinates of the exit point
    int CELL_SIZE;                            ///< Dynamic cell size based on map dimensions
    std::vector<Vector2> path;                ///< Calculated path from entry to exit
    
    /**
     * @brief Checks if coordinates are within the map boundaries
     * @param x X-coordinate to check
     * @param y Y-coordinate to check
     * @return true if the coordinates are valid, false otherwise
     */
    bool isValidCoordinate(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    
    /**
     * @brief Checks if there is a valid path from entry to exit
     * @return true if a valid path exists, false otherwise
     */
    bool hasValidPath() const;

public:
    /**
     * @brief Constructor for the Map class
     * @param w Width of the map in cells
     * @param h Height of the map in cells
     */
    Map(int w, int h);
    
    /**
     * @brief Copy constructor for the Map class
     * @param other Reference to the Map object to copy
     */
    Map(const Map& other);
    
    /**
     * @brief Gets the cell type at the specified coordinates
     * @param x X-coordinate of the cell
     * @param y Y-coordinate of the cell
     * @return The CellType at the specified coordinates
     */
    CellType getCellType(int x, int y) const;
    
    /**
     * @brief Places a tower at the specified coordinates
     * @param x X-coordinate for tower placement
     * @param y Y-coordinate for tower placement
     * @return true if the tower was successfully placed, false otherwise
     */
    bool placeTower(int x, int y);
    
    /**
     * @brief Draws the map on the screen
     * @param offsetX X-offset for drawing the map (default: 0)
     * @param offsetY Y-offset for drawing the map (default: 0)
     * @param cellSize Size of each cell in pixels (default: 0, auto-calculated)
     */
    void draw(int offsetX = 0, int offsetY = 0, int cellSize = 0) const;
    
    /**
     * @brief Gets the entry point coordinates
     * @return Vector2 containing the entry point coordinates
     */
    Vector2 getEntryPoint() const { return entryPoint; }
    
    /**
     * @brief Gets the exit point coordinates
     * @return Vector2 containing the exit point coordinates
     */
    Vector2 getExitPoint() const { return exitPoint; }
    
    /**
     * @brief Gets the width of the map
     * @return Width of the map in cells
     */
    int getWidth() const { return width; }
    
    /**
     * @brief Gets the height of the map
     * @return Height of the map in cells
     */
    int getHeight() const { return height; }
    
    /**
     * @brief Validates the map for gameplay
     * @return true if the map is valid, false otherwise
     * @details Checks if entry and exit points are set and if a valid path exists
     */
    bool validateMap() const;
    
    /**
     * @brief Sets the cell type at the specified coordinates
     * @param x X-coordinate of the cell
     * @param y Y-coordinate of the cell
     * @param type The CellType to set
     */
    void setCellType(int x, int y, CellType type);
    
    /**
     * @brief Gets the calculated path from entry to exit
     * @return Constant reference to the vector of path waypoints
     */
    const std::vector<Vector2>& getPath() const { return path; }
    
    /**
     * @brief Sets the path to a custom sequence of waypoints
     * @param path Vector of Vector2 points that define the path
     */
    void setPath(const std::vector<Vector2>& path);
    
    /**
     * @brief Calculates a valid path from entry to exit point
     */
    void calculatePath();
};
