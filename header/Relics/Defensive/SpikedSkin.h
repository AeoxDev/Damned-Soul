#pragma once
#include "EntityID.h"

namespace SPIKED_SKIN
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The damage returned when hit
	void Retaliation(void* data);
}