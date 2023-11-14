#pragma once
#include "EntityID.h"

namespace BACK_SHIELD
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Call to recharge the golden duck
	void Recharge(void* data);

	// Fully recharge
	void FullRecharge(void* data);

	// Nullify a weapon hit from behind
	void NullBackHit(void* data);
}