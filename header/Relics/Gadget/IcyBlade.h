#pragma once
#include "EntityID.h"

namespace ICY_BLADE
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The chance to freeze on hit
	void FreezeOnHit(void* data);
}