#pragma once
#include "EntityID.h"

namespace THRILL_SEEKER
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Damage function for Thrill Seeker
	void DamageAmplifier(void* data);
}