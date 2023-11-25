#pragma once
#include "EntityID.h"

namespace SUCCUBUS_CHARM
{
	// The owner
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	void DamageReduction(void* input);
	
	void Discount(void* data);
}