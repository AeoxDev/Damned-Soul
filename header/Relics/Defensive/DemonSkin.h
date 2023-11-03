#pragma once
#include "EntityID.h"

namespace DEMON_SKIN
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Damage Reduction function for Demon Skin
	void DecreaseDamageTaken(void* data);
}