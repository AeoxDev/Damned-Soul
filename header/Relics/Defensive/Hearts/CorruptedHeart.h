#pragma once
#include "EntityID.h"

namespace CORRUPTED_HEART
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Reduces the instant damage taken
	void DamageReduction(void* data);

	// Applies the DoT
	void ApplyDot(void* data);

	// The Increase Health function for Corrupted Heart
	void IncreaseHealth(void* data);
}