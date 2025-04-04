/**
 * @file Game.h
 * @brief Main game class that manages the tower defense game
 * @details Controls game states, UI, map management, tower placement,
 *          critter waves, and player resources
 */

#pragma once
#include "Map.h"
#include <memory>
#include <vector>
#include "CritterWave.h"
#include "TowerManager.h"
#include "CritterObserver.h"
#include "MapEditor.h"  // Include MapEditor header

/**
 * @enum GameState
 * @brief Defines the possible states of the game
 */
enum class GameState {
    MAP_PRESET_SELECTION,  ///< Selecting from predefined maps
    MAP_SELECTION,         ///< Custom map size selection
    MAP_EDITING,           ///< Editing a custom map
    PLAYING,               ///< Normal gameplay
    GAME_OVER              ///< Game is over (player lost)
};

/**
 * @class Game
 * @brief Main game controller class
 * @details Implements the CritterObserver interface to respond to critter events
 *          and manages all aspects of the tower defense game
 */
class Game : public CritterObserver {
private:
    std::unique_ptr<TowerManager> towerManager;     ///< Manages towers and currency
    GameState state;                                ///< Current game state
    Map* currentMap;                                ///< The active game map
    std::vector<std::pair<int, int>> mapSizes;      ///< Available map size options
    std::vector<std::unique_ptr<Map>> presetMaps;   ///< Collection of predefined maps
    int selectedPresetIndex = 0;                    ///< Currently selected preset map index
    int selectedSize;                               ///< Selected map size index
    std::string widthInput;                         ///< Stores the width input as a string
    std::string heightInput;                        ///< Stores the height input as a string
    bool isEditingWidth;                            ///< Flag to track which dimension is being edited
    std::unique_ptr<CritterWave> critterWave;       ///< Current wave of critters
    std::unique_ptr<MapEditor> mapEditor;           ///< Map editor for custom maps
    
    int currentWave = 0;                            ///< Current wave number
    int lives = 10;                                 ///< Player's remaining lives
    
    /**
     * @struct WaveNotification
     * @brief Manages the wave number notification display
     */
    struct WaveNotification {
        int waveNumber;     ///< Wave number to display
        float timer;        ///< Time remaining for notification
        float fadeDuration; ///< Duration of fade effect
    };
    
    WaveNotification waveNotification;              ///< Current wave notification
    std::string selectedTowerType;                  ///< Type of tower currently selected for placement
    float towerMenuHeight = 100;                    ///< Height of the tower selection menu
    float sideMenuWidth = 200;                      ///< Width of the side menu for tower info
    Tower* selectedTower = nullptr;                 ///< Currently selected tower
    
    /**
     * @struct Shot
     * @brief Represents a visual shot from a tower to a target
     */
    struct Shot {
        Vector2 start;      ///< Starting position of the shot
        Vector2 end;        ///< Ending position of the shot
        float timer;        ///< Duration the shot remains visible
        Color color;        ///< Color of the shot
    };
    
    std::vector<Shot> activeShots;                  ///< Collection of active tower shots
    
    /**
     * @struct RewardNotification
     * @brief Floating text notification for rewards/penalties
     */
    struct RewardNotification {
        Vector2 position;   ///< Position to display the notification
        int amount;         ///< Amount of currency gained/lost
        float timer;        ///< Duration the notification remains visible
    };
    
    std::vector<RewardNotification> rewardNotifications; ///< Active reward notifications

    /**
     * @brief Renders the map selection screen
     */
    void drawMapSelection() const;
    
    /**
     * @brief Handles user input for map selection
     */
    void handleMapSelection();
    
    /**
     * @brief Renders the tower selection menu
     */
    void drawTowerMenu() const;
    
    /**
     * @brief Processes clicks in the tower menu
     * @param mousePos Current mouse position
     */
    void handleTowerMenuClick(Vector2 mousePos);
    
    /**
     * @brief Checks if there are any active critters
     * @return True if there are active critters, false otherwise
     */
    bool hasActiveCritters() const;
    
    /**
     * @brief Renders the side menu
     */
    void drawSideMenu() const;
    
    /**
     * @brief Handles clicks on the side menu button
     * @param mousePos Current mouse position
     */
    void handleSideMenuButtonClick(Vector2 mousePos);
    
    /**
     * @brief Renders the "Next Wave" button in the side menu
     */
    void drawSideMenuButton() const;
    
    /**
     * @brief Renders the default side menu content
     */
    void drawSideMenuDefault() const;
    
    /**
     * @brief Renders selected tower information in the side menu
     */
    void drawTowerInfoInSideMenu() const;
    
    /**
     * @brief Renders the game over screen
     */
    void drawGameOver() const;
    
    /**
     * @brief Handles tower selection when clicking on the map
     * @param mousePos Current mouse position
     */
    void handleTowerSelection(Vector2 mousePos);
    
    /**
     * @brief Processes clicks in the tower info menu
     * @param mousePos Current mouse position
     */
    void handleTowerInfoMenuClick(Vector2 mousePos);
    
    /**
     * @brief Updates all towers' targeting and attacks
     */
    void updateTowers();
    
    /**
     * @brief Renders the visual shots from towers to targets
     */
    void drawTowerShots() const;

public:
    /**
     * @brief Constructor for the Game class
     * @details Initializes game state, resources, and UI elements
     */
    Game();
    
    /**
     * @brief Destructor for the Game class
     */
    ~Game();
    
    /**
     * @brief Displays the wave number notification
     */
    void showWaveNotification();
    
    /**
     * @brief Updates the wave notification timer
     */
    void updateWaveNotification();
    
    /**
     * @brief Renders the wave notification
     */
    void drawWaveNotification() const;
    
    /**
     * @brief Creates predefined maps for selection
     */
    void generatePresetMaps();
    
    /**
     * @brief Renders the preset map selection screen
     */
    void drawPresetSelection() const;
    
    /**
     * @brief Processes user input for preset map selection
     */
    void handlePresetSelection();
    
    /**
     * @brief Main update function for the game
     * @details Updates game objects, processes input, and manages state transitions
     */
    void update();
    
    /**
     * @brief Main render function for the game
     */
    void draw() const;
    
    /**
     * @brief Observer callback when a critter reaches the end of the path
     * @param critter Reference to the critter that reached the end
     * @details Reduces player lives and currency
     */
    void onCritterReachedEnd(const Critter& critter) override;
    
    /**
     * @brief Observer callback when a critter is defeated
     * @param critter Reference to the critter that was defeated
     * @details Awards currency to the player
     */
    void onCritterDefeated(const Critter& critter) override;
    
    /**
     * @brief Creates a floating reward/penalty notification
     * @param position Position to display the notification
     * @param amount Amount of currency gained/lost
     */
    void addRewardNotification(const Vector2& position, int amount);
    
    /**
     * @brief Starts the next wave of critters
     */
    void startNextWave();
    
    /**
     * @brief Resets player lives to the starting amount
     */
    void resetLives();
};
