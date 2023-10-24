#pragma once
#include "EntityFramework.h"
//struct Registry;
extern Registry registry;

/// destructionTiers: 0: no persistents destroyed, 1: player, 2: audioEngine, 3: everything
void UnloadEntities(int destructionTier = 0);