#pragma once
#include "EntityID.h"

namespace ADRENALINE_RUSH
{
	extern EntityID _OWNER;

	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	//The 
	void OnDamageTaken(void* data);
	
	void OnUpdate(void* data);

	// The Increase Attack Speed function for Thrill Seeker
	void AttackSpeedBoost(void* data);
}