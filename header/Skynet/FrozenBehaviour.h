#pragma once

struct FrozenBehaviour
{
	float frozenTimeAmount = 10.f; // time before frozen spawns an enemy
	float frozenCounter = 0.f;
	EnemyType type;
};