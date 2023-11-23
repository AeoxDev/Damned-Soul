#pragma once
#include "EntityID.h"

namespace FROZEN_MAUL
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Increase knockback stat function
	void IncreaseKnockback(void* data);

	// The chance to freeze on hit
	void FreezeOnHit(void* data);
}