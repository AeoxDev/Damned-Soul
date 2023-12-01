#pragma once
#include "EntityID.h"

namespace MEMBERSHIP
{
	// The owner
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Trigger every few seconds
	void Discount(void* data);
}