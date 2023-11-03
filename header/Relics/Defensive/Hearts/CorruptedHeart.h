#pragma once
#include "EntityID.h"

namespace CORRUPTED_HEART
{
	extern EntityID _OWNER;

	//// Get the Corrupted Heart's description
	//char* GetDescription();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Health function for Corrupted Heart
	void IncreaseHealth(void* data);
}