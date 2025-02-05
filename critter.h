#ifndef CRITTER_H
#define CRITTER_H

/**
 * @class Critter
 * @brief Represents a single enemy unit in the tower defense game
 */
class Critter {
    public:
        struct Position {   ///< Represents a 2D position on the map
            int x, y;
        };

    private:
        int hitPoints;       ///< Current health of the critter
        int strength;        ///< Damage dealt to player when reaching exit
        int speed;          ///< Movement speed of the critter
        int level;          ///< Level of the critter
        int reward;         ///< Coins awarded when killed
        bool reachedExit;   ///< Flag to track if critter reached exit
        Position position;  ///< Current position on the map

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
        Critter(int hp, int str, int spd, int lvl, int rwd, Position pos);

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
        Position getPosition() const;
        void setPosition(int x, int y);
};

#endif // CRITTER_H