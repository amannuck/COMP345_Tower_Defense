//
// Created by amann on 02/03/2025.
//

#ifndef CRITTEROBSERVER_H
#define CRITTEROBSERVER_H

#pragma once
#include "IObserver.h"

class Critter;

class CritterObserver: public IObserver {
public:
    virtual ~CritterObserver() = default;

    // Called when a critter reaches the end of the path
    virtual void onCritterReachedEnd(const Critter& critter) = 0;

    // Called when a critter is defeated
    virtual void onCritterDefeated(const Critter& critter) = 0;
};

#endif //CRITTEROBSERVER_H
