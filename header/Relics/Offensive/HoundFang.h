#pragma once
#include "EntityID.h"

namespace HOUND_FANG
{
	extern EntityID _OWNER;

	// Generate the description
	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Strength function for Hound Fang
	void IncreaseBaseDamage(void* data);
}