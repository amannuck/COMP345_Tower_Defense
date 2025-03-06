#include "CritterWave.h"
#include <iostream>

//CritterWave::CritterWave(int waveLevel, const std::vector<Vector2>& path)
//        : path(path), currentCritterIndex(0), spawnTimer(0), spawnDelay(1.5f) { // ✅ Store path
//
//    if (path.empty()) {
//        std::cerr << "ERROR: Path is empty! Cannot spawn critters." << std::endl;
//        return;
//    }
//
//    int critterCount = 5 + waveLevel;  // More critters as difficulty increases
//    for (int i = 0; i < critterCount; i++) {
//        float speed = 1.0f + (waveLevel * 0.1f);
//        float hp = 50.0f + (waveLevel * 10);
//        int reward = 10 + (waveLevel * 2);
//        int strength = 2 + waveLevel;
//
//        Critter critter(waveLevel, speed, hp, reward, strength, path);
//        critter.setPosition(path.front());  // ✅ Start at first path point
//        critters.push_back(critter);
//    }
//}
CritterWave::CritterWave(int waveLevel, const std::vector<Vector2>& path)
        : path(path), currentCritterIndex(0), spawnTimer(0), spawnDelay(1.5f) {

    if (path.empty()) {
        std::cerr << "ERROR: Path is empty! Cannot spawn critters." << std::endl;
        return;
    }

    int critterCount = 5 + waveLevel;
    for (int i = 0; i < critterCount; i++) {
        float speed = 0.03f + (waveLevel * 0.01f);
        float hp = 50.0f + (waveLevel * 10);
        int reward = 10 + (waveLevel * 2);
        int strength = 2 + waveLevel;

        Critter critter(waveLevel, speed, hp, reward, strength, path);
        critter.setPosition(path.front());

        critters.push_back(critter);
    }

    std::cout << "✅ CritterWave created! " << critters.size() << " critters initialized." << std::endl;
}




void CritterWave::update(float deltaTime) {
    spawnTimer += deltaTime;

    // Activate critters gradually
    if (currentCritterIndex < critters.size() && spawnTimer >= spawnDelay) {
        critters[currentCritterIndex].activate();
        std::cout << "🚀 Critter " << currentCritterIndex << " activated!" << std::endl;
        spawnTimer = 0;  // Reset timer
        currentCritterIndex++;
    }

    // Move all active critters
    for (auto& critter : critters) {
        if (critter.isActive()) {
            critter.move();
        }
    }
}


//void CritterWave::update(float deltaTime) {
//    spawnTimer += deltaTime;
//
//    // Ensure all critters are activated properly
//    for (int i = 0; i < critters.size(); i++) {
//        if (!critters[i].isActive()) {
//            critters[i].activate();
//            std::cout << "🚀 Critter " << i << " activated!" << std::endl;
//        }
//    }
//
//    // Move critters
//    for (auto& critter : critters) {
//        if (critter.isActive()) {
//            critter.move();
//        }
//    }
//}



//void CritterWave::draw() const {
//    std::cout << "✅ Calling currentWave->draw(). Critters in wave: " << critters.size() << std::endl;
//
//    int drawnCritters = 0;
//    for (auto& critter : critters) {
//        if (critter.isActive()) {
//            critter.draw();
//            drawnCritters++;
//        }
//    }
//
//    std::cout << "🎨 Drawing " << drawnCritters << " critters." << std::endl;
//}
void CritterWave::draw(int offsetX, int offsetY, int cellSize) const {
    int drawnCritters = 0;
    for (const auto& critter : critters) {
        if (critter.isActive()) {
            critter.draw(offsetX, offsetY, cellSize);
            drawnCritters++;
        }
    }
    std::cout << "🎨 Drawing " << drawnCritters << " critters." << std::endl;
}



bool CritterWave::allCrittersDefeated() const {
    return currentCritterIndex >= critters.size();
}

std::vector<Critter>& CritterWave::getCritters() {
    return critters;
}
