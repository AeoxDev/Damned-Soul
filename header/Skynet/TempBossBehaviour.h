#pragma once
struct TempBossBehaviour
{
	float updateInterval = 0.4f; // not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	const float attackStunDuration = 1.2f;
	int deathCounter = 0;

	float meleeDistance = 7.f;

	int hitBoxID = 0;

	bool parts[5] = { false, false, false,false,false };
	// index 0 is 
	// index 1 is 
	// index 2 is 
	// index 3 is 
	// index 4 is 

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

	TempBossBehaviour(int deathCount, int hitID) : deathCounter(deathCount), hitBoxID(hitID) {}
};