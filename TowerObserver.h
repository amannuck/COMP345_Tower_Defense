#pragma once
#include <raylib.h>
#include <string>

// Forward declarations
class Tower;

// Define tower event types
enum class TowerEventType {
    TOWER_FIRED,
    TOWER_UPGRADED,
    TOWER_SOLD,
    TOWER_PLACED
};

// Tower Observer interface
class ITowerObserver {
public:
    virtual ~ITowerObserver() = default;
    virtual void onTowerEvent(Tower* tower, TowerEventType eventType) = 0;
};

// Tower Subject interface
class ITowerSubject {
public:
    virtual ~ITowerSubject() = default;
    virtual void addObserver(ITowerObserver* observer) = 0;
    virtual void removeObserver(ITowerObserver* observer) = 0;
    virtual void notifyObservers(TowerEventType eventType) = 0;
};