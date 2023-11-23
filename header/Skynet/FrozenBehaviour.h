#pragma once

struct FrozenBehaviour
{
	float frozenTimeAmount = 15.f; // time before frozen spawns an enemy
	float frozenCounter = 0.f;
	EnemyType type;


};