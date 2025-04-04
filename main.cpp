/**
 * @file main.cpp
 * @brief Entry point for the tower defense game
 * @details Initializes the game window, sets up the game instance,
 *          and manages the main game loop.
 */

#include "Game.h"

/**
 * @brief Main function and entry point for the application
 * @return Exit code (0 for success)
 * @details Initializes the Raylib window, creates a Game instance,
 *          and runs the main game loop until the window is closed.
 */
int main() {
    // Define screen dimensions
    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    // Initialize window with Raylib
    InitWindow(screenWidth, screenHeight, "Tower Defense The Game");
    SetTargetFPS(60);
    
    // Create game instance
    Game game;
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update game state
        game.update();
        
        // Render frame
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.draw();
        EndDrawing();
    }
    
    // Clean up and exit
    CloseWindow();
    return 0;
}
