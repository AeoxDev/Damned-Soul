#pragma once
#include "EntityID.h"

namespace SOUL_EATER
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Damage Reduction function for Demon Skin
	void HealFromSouls(void* data);
}