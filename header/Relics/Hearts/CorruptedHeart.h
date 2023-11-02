#pragma once
#include "EntityID.h"

namespace CORRUPTED_HEART
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Health function for Corrupted Heart
	void IncreaseHealth(void* data);
}