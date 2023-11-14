#pragma once

struct LuciferBehaviour
{
	float updateInterval = 0.4f; // not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;

	int hitBoxID = 0;




	float spawnCounter = 10.f;
	float spawnTiming = 15.f;


	//how often pathfinding
	float updatePathCounter = 1.8f;
	float updatePathLimit = 0.5f;

	//pathfinding
	float dirX = 0.f;
	float dirZ = 0.f;
	float dir2X = 0.f;
	float dir2Z = 0.f;
	bool followPath = false;


	//trust me, these are needed. int errors
	float fx = 0.f;
	float fz = 0.f;




	// MELEE VARIABLES
	float attackTimer = 0.f; //used as counter
	float attackStunDurationCounter = 0.8f;
	const float attackStunDuration = 1.5f;
	float meleeDistance = 3.5f;
	int attackHitboxID = 2;
};