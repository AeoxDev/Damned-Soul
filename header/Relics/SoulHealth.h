#pragma once

namespace SOUL_HEALTH
{
	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Modifies the player's Health by the current soul count initially
	void ModifyPlayerHealthInitial(void* data);

	// Modifies the player's Health when souls count is altered
	void ModifyPlayerHealthUpdate(void* data);
}