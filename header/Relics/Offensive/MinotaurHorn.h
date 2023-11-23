#pragma once
#include "EntityID.h"

namespace MINOTAUR_HORN
{
	extern EntityID _OWNER;

	// Generate the description
	const char* Description();

	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// The Increase Strength function for Minotaur Horn
	void IncreaseStrengthAndKnockback(void* data);
}