// CritterFactory.h
#pragma once
#include "Critter.h"
#include <vector>
#include <memory>

// Abstract Factory for creating critters
class CritterFactory {
public:
    virtual ~CritterFactory() = default;
    
    // Create a critter based on the factory's implementation
    virtual Critter createCritter(const std::vector<Vector2>& path) const = 0;
    
    // Create a wave of critters
    std::vector<Critter> createWave(const std::vector<Vector2>& path, int count) const {
        std::vector<Critter> critters;
        for (int i = 0; i < count; i++) {
            critters.push_back(createCritter(path));
        }
        return critters;
    }
};

// Factory for creating basic critters (early waves)
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
        
        return Critter(waveLevel, speed, hp, reward, strength, path);
    }
};

// Factory for creating fast critters (medium waves)
class FastCritterFactory : public CritterFactory {
private:
    int waveLevel;
    
public:
    FastCritterFactory(int waveLevel) : waveLevel(waveLevel) {}
    
    Critter createCritter(const std::vector<Vector2>& path) const override {
        // Fast critters have higher speed but lower HP
        float speed = 60.0f + (waveLevel * 8.0f);
        float hp = 30.0f + (waveLevel * 7.0f);
        int reward = 15 + (waveLevel * 2);
        int strength = 1 + waveLevel;
        
        return Critter(waveLevel, speed, hp, reward, strength, path);
    }
};

// Factory for creating tank critters (later waves)
class TankCritterFactory : public CritterFactory {
private:
    int waveLevel;
    
public:
    TankCritterFactory(int waveLevel) : waveLevel(waveLevel) {}
    
    Critter createCritter(const std::vector<Vector2>& path) const override {
        // Tank critters have high HP but lower speed
        float speed = 25.0f + (waveLevel * 3.0f);
        float hp = 100.0f + (waveLevel * 15.0f);
        int reward = 20 + (waveLevel * 3);
        int strength = 3 + waveLevel;
        
        return Critter(waveLevel, speed, hp, reward, strength, path);
    }
};

// Factory for creating boss critters (milestone waves)
class BossCritterFactory : public CritterFactory {
private:
    int waveLevel;
    
public:
    BossCritterFactory(int waveLevel) : waveLevel(waveLevel) {}
    
    Critter createCritter(const std::vector<Vector2>& path) const override {
        // Boss critters are very strong with high HP and medium speed
        float speed = 35.0f + (waveLevel * 4.0f);
        float hp = 200.0f + (waveLevel * 25.0f);
        int reward = 50 + (waveLevel * 5);
        int strength = 5 + waveLevel * 2;
        
        return Critter(waveLevel, speed, hp, reward, strength, path);
    }
};

// Factory creator - decides which factory to use based on wave number
class CritterFactoryCreator {
public:
    static std::unique_ptr<CritterFactory> createFactory(int waveLevel) {
        // For milestone waves (multiples of 5), create boss critters
        if (waveLevel % 5 == 0) {
            return std::make_unique<BossCritterFactory>(waveLevel);
        }
        // For early waves (1-3), create basic critters
        else if (waveLevel <= 3) {
            return std::make_unique<BasicCritterFactory>(waveLevel);
        }
        // For medium waves (4-7), create fast critters
        else if (waveLevel <= 7) {
            return std::make_unique<FastCritterFactory>(waveLevel);
        }
        // For later waves (8+), create tank critters
        else {
            return std::make_unique<TankCritterFactory>(waveLevel);
        }
    }
};
