#include "Tower.h"
#include <cmath>
#include <algorithm>

Tower::Tower(float range, float power, float fireRate, int buyCost, int upgradeCost,
             const std::string& name, const Color& color)
        : level(1), range(range), power(power), fireRate(fireRate), lastShotTime(0),
          position({0, 0}), buyCost(buyCost), upgradeCost(upgradeCost), name(name), color(color) {}

Tower::~Tower() {}

void Tower::upgrade() {
    level++;
    range *= 1.2f;
    power *= 1.3f;
    fireRate *= 1.1f;
    upgradeCost = static_cast<int>(upgradeCost * 1.5f);
    
    // Notify observers
    notifyObservers(TowerEventType::TOWER_UPGRADED);
}

int Tower::getRefundValue() const {
    float totalCost = buyCost;
    float refundRatio = 0.7f;  // 70% refund
    return static_cast<int>(totalCost * refundRatio);
}

void Tower::draw() const {
    // Draw tower
    DrawCircle(position.x, position.y, 15, color);

    // Draw level indicator
    std::string levelText = "Lvl " + std::to_string(level);
    DrawText(levelText.c_str(), position.x - 10, position.y - 25, 10, BLACK);
}

bool Tower::canShoot() const {
    return GetTime() - lastShotTime >= 1.0f / fireRate;
}

void Tower::resetShotTimer() {
    lastShotTime = GetTime();
    
    // Notify observers when tower fires
    notifyObservers(TowerEventType::TOWER_FIRED);
}

void Tower::setPosition(Vector2 pos) {
    position = pos;
    
    // Notify observers when tower is placed
    notifyObservers(TowerEventType::TOWER_PLACED);
}

// Observer pattern methods implementation
void Tower::addObserver(ITowerObserver* observer) {
    if (observer) {
        // Check if observer already exists
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it == observers.end()) {
            observers.push_back(observer);
        }
    }
}

void Tower::removeObserver(ITowerObserver* observer) {
    if (observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
}

void Tower::notifyObservers(TowerEventType eventType) {
    for (auto observer : observers) {
        observer->onTowerEvent(this, eventType);
    }
}

BasicTower::BasicTower()
        : Tower(150.0f, 10.0f, 1.0f, 100, 50, "Basic Tower", RED) {}

void BasicTower::upgrade() {
    Tower::upgrade();
    // Basic tower gets extra power on upgrade
    power *= 1.1f;
    // No need to notify observers here as it's already done in Tower::upgrade()
}

AreaTower::AreaTower()
        : Tower(120.0f, 8.0f, 0.8f, 150, 75, "Area Tower", BLUE), areaRadius(40.0f) {}

void AreaTower::upgrade() {
    Tower::upgrade();
    areaRadius *= 1.2f;
    // No need to notify observers here as it's already done in Tower::upgrade()
}

SlowTower::SlowTower()
        : Tower(130.0f, 5.0f, 1.2f, 125, 60, "Slow Tower", YELLOW),
          slowEffect(0.3f), slowDuration(2.0f) {}

void SlowTower::upgrade() {
    Tower::upgrade();
    slowEffect *= 1.15f;
    slowDuration *= 1.1f;
    // No need to notify observers here as it's already done in Tower::upgrade()
}