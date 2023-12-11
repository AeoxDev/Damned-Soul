#pragma once
#include "EntityID.h"

namespace WENDIGO_TALISMAN
{
	extern EntityID _OWNER;

	// Generate the description
	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Call to reset healing capabilities
	void ResetHeal(void* input);

	// The heal on charge attack function of wendigo talisman
	void HealOnChargeAttack(void* data);
}