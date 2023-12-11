#pragma once
#include "EntityID.h"

void ParticleAtEntityLocation(const EntityID& origin, const float duration, void* funcStart);
void ParticleAtEntityLocationFollow(const EntityID& origin, const float duration, void* funcStart);