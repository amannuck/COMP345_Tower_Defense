// CritterFactory.h
#pragma once
#include "Critter.h"
#include <vector>
#include <memory>
#include <random>

// Abstract Factory for creating critters
class CritterFactory {
public:
    virtual ~CritterFactory() = default;

    // Create a critter based on the factory's implementation
    virtual Critter createCritter(const std::vector<Vector2>& path) const = 0;

    // Create a wave of critters
    virtual std::vector<Critter> createWave(const std::vector<Vector2>& path, int count) const {
        std::vector<Critter> critters;
        for (int i = 0; i < count; i++) {
            critters.push_back(createCritter(path));
        }
        return critters;
    }
};

// Factory for creating basic critters
class BasicCritterFactory : public CritterFactory {
private:
    int waveLevel;

public:
    BasicCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 40.0f + (waveLevel * 5.0f);
        float hp = 50.0f + (waveLevel * 10.0f);
        int reward = 10 + (waveLevel * 2);
        int strength = 2 + waveLevel;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Basic");
    }
};

// Factory for creating fast critters
class FastCritterFactory : public CritterFactory {
private:
    int waveLevel;

public:
    FastCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 60.0f + (waveLevel * 8.0f);
        float hp = 30.0f + (waveLevel * 7.0f);
        int reward = 15 + (waveLevel * 2);
        int strength = 1 + waveLevel;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Fast");
    }
};

// Factory for creating tank critters
class TankCritterFactory : public CritterFactory {
private:
    int waveLevel;

public:
    TankCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 25.0f + (waveLevel * 3.0f);
        float hp = 100.0f + (waveLevel * 15.0f);
        int reward = 20 + (waveLevel * 3);
        int strength = 3 + waveLevel;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Tank");
    }
};

// Factory for creating boss critters
class BossCritterFactory : public CritterFactory {
private:
    int waveLevel;

public:
    BossCritterFactory(int waveLevel) : waveLevel(waveLevel) {}

    Critter createCritter(const std::vector<Vector2>& path) const override {
        float speed = 35.0f + (waveLevel * 4.0f);
        float hp = 200.0f + (waveLevel * 25.0f);
        int reward = 50 + (waveLevel * 5);
        int strength = 5 + waveLevel * 2;

        return Critter(waveLevel, speed, hp, reward, strength, path, "Boss");
    }
};

// Composite factory that creates mixed waves
// Factory for creating mixed waves of critters
class MixedCritterFactory : public CritterFactory {
private:
    int waveLevel;
    std::vector<std::unique_ptr<CritterFactory>> factories;
    mutable std::mt19937 rng; // Make rng mutable so it can be modified in const methods

public:
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

    Critter createCritter(const std::vector<Vector2>& path) const override {
        // Randomly select a factory (excluding boss factory for non-milestone waves)
        std::uniform_int_distribution<size_t> dist(0, factories.size() - 1);
        size_t index = dist(rng);
        return factories[index]->createCritter(path);
    }

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
    // Ratios change based on wave level to make waves more interesting
    float getBasicRatio() const {
        // Basic critters become less common as waves progress
        return std::max(0.5f - (waveLevel * 0.02f), 0.1f);
    }

    float getFastRatio() const {
        // Fast critters peak in mid waves
        if (waveLevel <= 5) return 0.2f + (waveLevel * 0.02f);
        if (waveLevel <= 15) return 0.3f;
        return 0.2f;
    }

    float getTankRatio() const {
        // Tank critters become more common in later waves
        return std::min(0.1f + (waveLevel * 0.01f), 0.4f);
    }

    float getBossRatio() const {
        // Boss critters appear in milestone waves
        return (waveLevel % 5 == 0) ? 0.1f + (waveLevel * 0.01f) : 0.0f;
    }
};

// Factory creator - decides which factory to use based on wave number
class CritterFactoryCreator {
public:
    static std::unique_ptr<CritterFactory> createFactory(int waveLevel) {
        // For early waves (1-3), use basic factory only
        if (waveLevel <= 2) {
            return std::make_unique<BasicCritterFactory>(waveLevel);
        }
        // For all other waves, use mixed factory
        return std::make_unique<MixedCritterFactory>(waveLevel);
    }
};