/**
 * @file MapEditor.h
 * @brief Defines the MapEditor class for creating and modifying maps
 * @details This class provides an interface for users to interactively create
 *          and edit maps for the tower defense game.
 */

#pragma once
#include "Map.h"

/**
 * @class MapEditor
 * @brief Interactive editor for creating tower defense maps
 * @details Provides tools for placing different cell types, handles user input,
 *          and manages the map creation process with validation feedback.
 */
class MapEditor {
private:
    Map* currentMap;     ///< Pointer to the map being edited
    CellType currentTool; ///< Currently selected cell type tool
    bool isDragging;     ///< Tracks whether the user is dragging to place multiple cells

public:
    /**
     * @brief Constructor for the MapEditor class
     * @param width Width of the map to be created
     * @param height Height of the map to be created
     */
    MapEditor(int width, int height);
    
    /**
     * @brief Destructor for the MapEditor class
     */
    ~MapEditor();
    
    /**
     * @brief Updates the map editor state based on user input
     * @details Handles mouse and keyboard input for map editing
     */
    void update();
    
    /**
     * @brief Renders the map editor interface
     * @details Draws the current map, tool selection UI, and validation status
     */
    void draw();
    
    /**
     * @brief Gets the map being edited
     * @return Pointer to the current map
     */
    Map* getMap() const { return currentMap; }
};
