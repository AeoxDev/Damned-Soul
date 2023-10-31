#pragma once

namespace SOUL_POWER
{
	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Modifies the player's Strength by the current soul count initially
	void ModifyPlayerStrengthInitial(void* data);

	// Modifies the player's Strength when souls count is altered
	void ModifyPlayerStrengthUpdate(void* data);
}