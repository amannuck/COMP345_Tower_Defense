//
// Created by amann on 27/02/2025.
//

#ifndef IOBSERVER_H
#define IOBSERVER_H

// IObserver.h
#pragma once

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

#pragma once
#include "IObserver.h"
#include <vector>
#include <algorithm>

class IObservable {
private:
    std::vector<IObserver*> observers;

public:
    virtual ~IObservable() = default;

    void addObserver(IObserver* observer) {
        observers.push_back(observer);
    }

    void removeObserver(IObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers() {
        for (auto observer : observers) {
            observer->update();
        }
    }
};



#endif //IOBSERVER_H
