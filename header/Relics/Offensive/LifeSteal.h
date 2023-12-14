#pragma once
#include "EntityID.h"

namespace LIFE_STEAL
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Function to reset the healing soft cap of life steal
	void ResetSoftCap(void* data);

	// The Increase Damage Reduction function for Demon Skin
	void HealFromDamage(void* data);
}