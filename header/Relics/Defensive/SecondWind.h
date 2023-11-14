#pragma once
#include "EntityID.h"

namespace SECOND_WIND
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Trigger on lethal damage, set health to something else
	void Revive(void* data);
}
