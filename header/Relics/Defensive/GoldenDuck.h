#pragma once
#include "EntityID.h"

namespace GOLDEN_DUCK
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Call to recharge the golden duck
	void Recharge(void* data);

	// Fully recharge
	void FullRecharge(void* data);

	// The damage returned when hit
	void NullHazardDot(void* data);
}