// main.cpp
#include "Game.h"

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "Tower Defense The Game");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        game.update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}