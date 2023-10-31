#pragma once
#include "EntityID.h"

namespace SOUL_POWER
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The modify damage function
	void ModifyStrength(void* data);
}