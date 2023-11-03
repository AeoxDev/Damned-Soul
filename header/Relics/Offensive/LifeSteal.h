#pragma once
#include "EntityID.h"

namespace LIFE_STEAL
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Damage Reduction function for Demon Skin
	void HealFromDamage(void* data);
}