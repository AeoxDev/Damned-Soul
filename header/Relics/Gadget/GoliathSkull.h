#pragma once
#include "EntityID.h"

namespace GOLIATH_SKULL
{
	extern EntityID _OWNER;

	// Generate the description
	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Damage function for Goliath Skull
	void IncreaseAttackDamage(void* data);
}