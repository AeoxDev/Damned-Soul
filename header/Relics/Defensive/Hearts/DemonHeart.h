#pragma once
#include "EntityID.h"

namespace DEMON_HEART
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Health function for Demon Heart
	void IncreaseHealth(void* data);
}