#pragma once

namespace SOUL_HEALTH
{
	// Modifies the player's Health by the current soul count initially
	void ModifyPlayerHealthInitial(void* data);

	// Modifies the player's Health when souls count is altered
	void ModifyPlayerHealthUpdate(void* data);
}