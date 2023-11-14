#pragma once
#include "EntityID.h"

namespace SOUL_HEALTH
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The modify health function
	void ModifyHealth(void* data);
}