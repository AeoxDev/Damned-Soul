#pragma once
#include "EntityID.h"

namespace SEEING_DOUBLE
{
	// The owner
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Function to disable the coupon upon leaving the shop or buying something
	void Disable(void* data);

	// Trigger every few seconds
	void NoCostOnThings(void* data);
}