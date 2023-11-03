#pragma once
#include "EntityID.h"

namespace RECKLESS
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Damage function for Reckless
	void DamageAmplifier(void* data);
}