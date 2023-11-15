#pragma once
#include "EntityID.h"

namespace FROST_FIRE
{
	// The owner
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Function to reset frost fire between levels
	void Reset(void* data);

	// Trigger every few seconds
	void PushBackAndDamage(void* data);
}