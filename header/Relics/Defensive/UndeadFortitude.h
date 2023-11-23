#pragma once
#include "EntityID.h"

namespace UNDEAD_FORTITUDE
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Damage Reduction function for Demon Skin
	void DecreaseDamageTaken(void* data);
}
