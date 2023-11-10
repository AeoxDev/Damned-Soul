#pragma once
#include "EntityID.h"

namespace DASH_ATTACK
{
	// The relic's owner
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Put the damage over time effect on the enemy that was hit
	void DealDamageWhenDashing(void* data);
}