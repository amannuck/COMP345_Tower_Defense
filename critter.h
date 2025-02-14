 #ifndef CRITTER_H
 #define CRITTER_H

 using namespace std;
 #include <utility>
 #include "mapgen.h"

 /**
  * @class Critter
  * @brief Represents a single enemy unit in the tower defense game
  */
 class Critter {
     private:
         int hitPoints;       ///< Current health of the critter
         int strength;        ///< Damage dealt to player when reaching exit
         int speed;          ///< Movement speed of the critter
         int level;          ///< Level of the critter
         int reward;         ///< Coins awarded when killed
         bool reachedExit;   ///< Flag to track if critter reached exit
         pair<int, int> position;  ///< Current position on the map
         const Map* map;     ///< Pointer to the map object

     public:
         /**
          * @brief Construct a new Critter object
          * @param hp Initial hit points
          * @param str Strength (damage to player)
          * @param spd Movement speed
          * @param lvl Critter level
          * @param rwd Coin reward when killed
          * @param pos Initial position
          */
         Critter(int hp, int str, int spd, int lvl, int rwd, pair<int, int> pos, const Map* gameMap);

         // Movement and combat methods
         void move();                    ///< Move towards exit point
         void takeDamage(int damage);    ///< Take damage from towers
         bool isDead() const;            ///< Check if critter is dead

         // Getter and setter methods
         int getHitPoints() const;
         int getStrength() const;
         int getSpeed() const;
         int getLevel() const;
         int getReward() const;
         bool hasReachedExit() const;
         pair<int, int> getPosition() const;
         void setPosition(int x, int y);
 };

 #endif // CRITTER_H