#pragma once
#include "EntityID.h"

namespace LAST_STAND
{
	extern EntityID _OWNER;

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Call to recharge the golden duck
	void Recharge(void* data);

	// Fully recharge
	void FullRecharge(void* data);

	// Trigger on lethal damage, set health to something else
	void ResistDamage(void* data);
}
