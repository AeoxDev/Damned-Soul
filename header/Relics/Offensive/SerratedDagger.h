#pragma once
#include "EntityID.h"

namespace SERRATED_DAGGER
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Charge Rate function of Serrated Dagger
	void IncreaseChargeRate(void* data);
}