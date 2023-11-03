#pragma once
#include "EntityID.h"

namespace ADVANCED_FIGHTING
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Crit Chance function of Advanced Fighting
	void CritChance(void* data);
}