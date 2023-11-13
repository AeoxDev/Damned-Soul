#pragma once

struct LuciferBehaviour
{
	float updateInterval = 0.4f; // not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	const float attackStunDuration = 1.2f;

	int hitBoxID = 0;




	float spawnCounter = 0.f;
	float spawnTiming = 5.f;



};