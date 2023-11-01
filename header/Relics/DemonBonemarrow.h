#pragma once
#include "EntityID.h"

namespace DEMON_BONEMARROW
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Strength function for Demon Bonemarrow
	void IncreasePlayerStrength(void* data);
}