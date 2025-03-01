/**
 * @file main.cpp
 * @brief Tower Defense game with SFML GUI.
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include "mapgen.h"
#include "tower.h"
#include "CritterGroup.h"

using namespace std;

const int TILE_SIZE = 40;  // Size of each grid tile in pixels

/**
 * @brief Renders the game map and objects using SFML.
 * @param window SFML window reference.
 * @param map The game map object.
 * @param towers The list of placed towers.
 * @param critters The list of active critters.
 */
void renderMap(sf::RenderWindow &window, Map &map, vector<Tower*> &towers, vector<Critter> &critters) {
    window.clear();

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);

            // Set colors based on cell type
            if (map.isPath(x, y)) {
                tile.setFillColor(sf::Color(150, 75, 0)); // Brown for path
            } else {
                tile.setFillColor(sf::Color(50, 205, 50)); // Green for scenery
            }
            window.draw(tile);
        }
    }

    // Draw towers
    for (Tower* tower : towers) {
        sf::CircleShape towerShape(TILE_SIZE / 2 - 5);
        towerShape.setPosition(tower->getX() * TILE_SIZE + 5, tower->getY() * TILE_SIZE + 5);
        towerShape.setFillColor(sf::Color::Blue);
        window.draw(towerShape);
    }

    // Draw critters
    for (const Critter &critter : critters) {
        sf::CircleShape critterShape(TILE_SIZE / 2 - 8);
        critterShape.setPosition(critter.getPosition().first * TILE_SIZE + 8, critter.getPosition().second * TILE_SIZE + 8);
        critterShape.setFillColor(sf::Color::Red);
        window.draw(critterShape);
    }

    window.display();
}

int main() {
    // Initialize the game map (10x10)
    Map gameMap(10, 10);
    gameMap.generateRandomMap();

    // Tower list
    vector<Tower*> towers;

    // Critter management
    CritterGroup group(&gameMap);
    int numCritters = group.generateWave();

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(400, 400), "Tower Defense Game");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle mouse click to place a tower
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / TILE_SIZE;
                int y = event.mouseButton.y / TILE_SIZE;
                placeTowerInteractive(gameMap, towers, x, y);
            }
        }

        // Game logic: move critters and attack them
        group.spawnNextCritter();
        vector<Critter> crittersCopy = group.getActiveCritters();  // Get active critters

        for (Tower* tower : towers) {
            tower->attack(crittersCopy);
        }

        group.moveAllCritters([](int damage) {
            cout << "A critter reached the exit! Player takes " << damage << " damage!\n";
        });

        // Render game objects
        renderMap(window, gameMap, towers, crittersCopy);
    }

    // Free memory (delete dynamically allocated towers)
    for (Tower* tower : towers) {
        delete tower;
    }

    return 0;
}

