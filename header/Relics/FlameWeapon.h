#pragma once

namespace FLAME_WEAPON
{
	// Call to set active for a specific entity (usually player)
	void Initialize(void* input);

	// Put the damage over time effect on the enemy that was hit
	void PlaceDamageOverTime(void* data);
}