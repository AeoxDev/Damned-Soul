#pragma once
#include "EntityID.h"

namespace SOUL_SPEED
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The modify speed function
	void ModifySpeed(void* data);
}