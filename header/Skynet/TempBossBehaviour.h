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

	int hitBoxID = 0;

	bool parts[5] = { false, false, false,false,false };

	TempBossBehaviour(int deathCount, int hitID) : deathCounter(deathCount), hitBoxID(hitID) {}
};