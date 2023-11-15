#pragma once
#include "EntityID.h"

namespace MOLTEN_HEART
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Health function for Molten Heart
	void IncreaseHealth(void* data);
}