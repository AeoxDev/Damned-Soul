#pragma once
#include "EntityID.h"

namespace QUICK_REFLEXES
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Dash Count function of Double Trouble
	void IncreaseDashCount(void* data);
}