#pragma once
#include "EntityID.h"

namespace PAIN_MIRROR
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The damage returned when hit
	void Retaliation(void* data);
}