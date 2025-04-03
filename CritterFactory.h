/**
 * @file CritterFactory.h
 * @brief Defines the factory classes for creating different types of critters
 * @details This file implements the Abstract Factory pattern for creating various
 *          types of critters including basic, fast, tank, and boss variants. It also
 *          provides a mixed factory for creating waves with multiple critter types.
 */

#pragma once
#include "Critter.h"
#include <vector>
#include <memory>
#include <random>

/**
 * @class CritterFactory
 * @brief Abstract Factory for creating critters
 * @details Defines the interface for all critter factory classes. Factories can create
 *          individual critters or entire waves of critters.
 */
class CritterFactory {
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~CritterFactory() = default;

    /**
     * @brief Create a critter based on the factory's implementation
     * @param path Vector of waypoints that the critter will follow
     * @return A new Critter instance configured according to factory specifications
     */
    virtual Critter createCritter(const std::vector<Vector2>& path) const = 0;

    /**
     * @brief Create a wave of critters
     * @param path Vector of waypoints that all critters in the wave will follow
     * @param count Number of critters to create in the wave
     * @return Vector containing the specified number of critters
     */
    virtual std::vector<Critter> createWave(const std::vector<Vector2>& path, int count) const {
        std::vector<Critter> critters;
        for (int i = 0; i < count; i++) {
            critters.push_back(createCritter(path));
        }
        return critters;
    }
};

/**
 * @class BasicCritterFactory
 * @brief Factory for creating basic critters
 * @details Creates balanced critters with moderate speed and health.
 *          Their attributes scale with the wave level.
 */
class BasicCritterFactory : public CritterFactory {
private:
    int waveLevel; ///< Current wave level, affects critter attributes

public:
    /**
     * @brief Constructor for BasicCritterFactory
     * @param waveLevel Current wave level, used to scale critter attributes
     */
    BasicCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    /**
     * @brief Create a basic critter with attributes scaled to the wave level
     * @param path Vector of waypoints that the critter will follow
     * @return A basic critter with balanced attributes
     */
    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 40.0f + (waveLevel * 5.0f);
        float hp = 50.0f + (waveLevel * 10.0f);
        int reward = 10 + (waveLevel * 2);
        int strength = 2 + waveLevel;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Basic");
    }
};

/**
 * @class FastCritterFactory
 * @brief Factory for creating fast critters
 * @details Creates critters with high speed but lower health.
 *          Their attributes scale with the wave level.
 */
class FastCritterFactory : public CritterFactory {
private:
    int waveLevel; ///< Current wave level, affects critter attributes

public:
    /**
     * @brief Constructor for FastCritterFactory
     * @param waveLevel Current wave level, used to scale critter attributes
     */
    FastCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    /**
     * @brief Create a fast critter with attributes scaled to the wave level
     * @param path Vector of waypoints that the critter will follow
     * @return A fast critter with high speed but lower health
     */
    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 60.0f + (waveLevel * 8.0f);
        float hp = 30.0f + (waveLevel * 7.0f);
        int reward = 15 + (waveLevel * 2);
        int strength = 1 + waveLevel;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Fast");
    }
};

/**
 * @class TankCritterFactory
 * @brief Factory for creating tank critters
 * @details Creates critters with high health but lower speed.
 *          Their attributes scale with the wave level.
 */
class TankCritterFactory : public CritterFactory {
private:
    int waveLevel; ///< Current wave level, affects critter attributes

public:
    /**
     * @brief Constructor for TankCritterFactory
     * @param waveLevel Current wave level, used to scale critter attributes
     */
    TankCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    /**
     * @brief Create a tank critter with attributes scaled to the wave level
     * @param path Vector of waypoints that the critter will follow
     * @return A tank critter with high health but lower speed
     */
    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 25.0f + (waveLevel * 3.0f);
        float hp = 100.0f + (waveLevel * 15.0f);
        int reward = 20 + (waveLevel * 3);
        int strength = 3 + waveLevel;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Tank");
    }
};

/**
 * @class BossCritterFactory
 * @brief Factory for creating boss critters
 * @details Creates powerful critters with high health and strength.
 *          Their attributes scale significantly with the wave level.
 */
class BossCritterFactory : public CritterFactory {
private:
    int waveLevel; ///< Current wave level, affects critter attributes

public:
    /**
     * @brief Constructor for BossCritterFactory
     * @param waveLevel Current wave level, used to scale critter attributes
     */
    BossCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    /**
     * @brief Create a boss critter with attributes scaled to the wave level
     * @param path Vector of waypoints that the critter will follow
     * @return A boss critter with high health and strength
     */
    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 35.0f + (waveLevel * 4.0f);
        float hp = 200.0f + (waveLevel * 25.0f);
        int reward = 50 + (waveLevel * 5);
        int strength = 5 + waveLevel * 2;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Boss");
    }
};

/**
 * @class MixedCritterFactory
 * @brief Composite factory that creates mixed waves of different critter types
 * @details Manages multiple critter factories to create waves with varying
 *          compositions of critter types based on the wave level.
 */
class MixedCritterFactory : public CritterFactory {
private:
    int waveLevel; ///< Current wave level, affects critter attributes and wave composition
    std::vector<std::unique_ptr<CritterFactory>> factories; ///< Collection of critter factories
    mutable std::mt19937 rng; ///< Random number generator, mutable to allow modification in const methods

public:
    /**
     * @brief Constructor for MixedCritterFactory
     * @param waveLevel Current wave level, used to determine wave composition and critter attributes
     */
    MixedCritterFactory(int waveLevel) : waveLevel(waveLevel), rng(std::random_device()()) {
        // Initialize all possible factories
        factories.push_back(std::make_unique<BasicCritterFactory>(waveLevel));
        factories.push_back(std::make_unique<FastCritterFactory>(waveLevel));
        factories.push_back(std::make_unique<TankCritterFactory>(waveLevel));

        // For milestone waves, include boss critters
        if (waveLevel % 5 == 0) {
            factories.push_back(std::make_unique<BossCritterFactory>(waveLevel));
        }
    }

    /**
     * @brief Create a random critter from the available factory types
     * @param path Vector of waypoints that the critter will follow
     * @return A random critter from one of the available factory types
     */
    Critter createCritter(const std::vector<Vector2>& path) const override {
        // Randomly select a factory (excluding boss factory for non-milestone waves)
        std::uniform_int_distribution<size_t> dist(0, factories.size() - 1);
        size_t index = dist(rng);
        return factories[index]->createCritter(path);
    }

    /**
     * @brief Create a wave with a mix of different critter types
     * @param path Vector of waypoints that all critters in the wave will follow
     * @param count Total number of critters to create in the wave
     * @return Vector containing a mix of different critter types
     * @details The composition of the wave is determined by the wave level,
     *          with different ratios of critter types for different wave levels.
     */
    std::vector<Critter> createWave(const std::vector<Vector2>& path, int count) const override {
        std::vector<Critter> critters;

        // Calculate composition based on wave level
        int basicCount = static_cast<int>(count * getBasicRatio());
        int fastCount = static_cast<int>(count * getFastRatio());
        int tankCount = static_cast<int>(count * getTankRatio());
        int bossCount = (waveLevel % 5 == 0) ? static_cast<int>(count * getBossRatio()) : 0;

        // Adjust counts to ensure total equals 'count'
        int remaining = count - (basicCount + fastCount + tankCount + bossCount);
        basicCount += remaining; // Add remaining to basic

        // Create critters using appropriate factories
        for (int i = 0; i < basicCount; i++) {
            critters.push_back(factories[0]->createCritter(path));
        }
        for (int i = 0; i < fastCount; i++) {
            critters.push_back(factories[1]->createCritter(path));
        }
        for (int i = 0; i < tankCount; i++) {
            critters.push_back(factories[2]->createCritter(path));
        }
        if (bossCount > 0) {
            for (int i = 0; i < bossCount; i++) {
                critters.push_back(factories[3]->createCritter(path));
            }
        }

        return critters;
    }

private:
    /**
     * @brief Calculate the ratio of basic critters for the current wave level
     * @return Ratio of basic critters as a float between 0.1 and 0.5
     * @details Basic critters become less common as waves progress
     */
    float getBasicRatio() const {
        // Basic critters become less common as waves progress
        return std::max(0.5f - (waveLevel * 0.02f), 0.1f);
    }

    /**
     * @brief Calculate the ratio of fast critters for the current wave level
     * @return Ratio of fast critters as a float between 0.2 and 0.3
     * @details Fast critters peak in mid waves
     */
    float getFastRatio() const {
        // Fast critters peak in mid waves
        if (waveLevel <= 5) return 0.2f + (waveLevel * 0.02f);
        if (waveLevel <= 15) return 0.3f;
        return 0.2f;
    }

    /**
     * @brief Calculate the ratio of tank critters for the current wave level
     * @return Ratio of tank critters as a float between 0.1 and 0.4
     * @details Tank critters become more common in later waves
     */
    float getTankRatio() const {
        // Tank critters become more common in later waves
        return std::min(0.1f + (waveLevel * 0.01f), 0.4f);
    }

    /**
     * @brief Calculate the ratio of boss critters for the current wave level
     * @return Ratio of boss critters as a float, 0.0 for non-milestone waves
     * @details Boss critters only appear in milestone waves (multiples of 5)
     */
    float getBossRatio() const {
        // Boss critters appear in milestone waves
        return (waveLevel % 5 == 0) ? 0.1f + (waveLevel * 0.01f) : 0.0f;
    }
};

/**
 * @class CritterFactoryCreator
 * @brief Factory creator that determines which factory type to use based on wave number
 * @details Acts as a facade for creating the appropriate factory based on game progression.
 */
class CritterFactoryCreator {
public:
    /**
     * @brief Create the appropriate factory for the current wave level
     * @param waveLevel Current wave level
     * @return A unique pointer to the appropriate CritterFactory implementation
     * @details Returns a BasicCritterFactory for early waves (1-2) and a
     *          MixedCritterFactory for all later waves.
     */
    static std::unique_ptr<CritterFactory> createFactory(int waveLevel) {
        // For early waves (1-3), use basic factory only
        if (waveLevel <= 2) {
            return std::make_unique<BasicCritterFactory>(waveLevel);
        }
        // For all other waves, use mixed factory
        return std::make_unique<MixedCritterFactory>(waveLevel);
    }
};
