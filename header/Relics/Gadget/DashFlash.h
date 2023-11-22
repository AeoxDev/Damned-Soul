#pragma once
#include "EntityID.h"

namespace DASH_FLASH
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Health function for Demon Heart
	void IncreaseDashDistance(void* data);
}