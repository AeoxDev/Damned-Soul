#pragma once
#include "EntityFramework.h"

extern Registry registry;

//When UnloadEntities is called, every entity with persistency EQUAL TO OR BELOW the value specified will be unloaded
void UnloadEntities(ENTITY_PERSISTENCY_TIER destructionTier = ENT_PERSIST_LOWEST);