#pragma once
#include "EntityID.h"

namespace CRACKED_LENSE
{
	extern EntityID _OWNER;

	// Generate the description
	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Strength function for Demon Bonemarrow
	void IncreaseStrength(void* data);
}