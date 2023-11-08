#pragma once

namespace FROST_FIRE
{
	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Function to mark Frost Fire as available
	void SetAvailable(void* data);

	// Trigger on Damage Taken, with an internal check if the player's health is low enough
	void PushBackAndFreeze(void* data);
}