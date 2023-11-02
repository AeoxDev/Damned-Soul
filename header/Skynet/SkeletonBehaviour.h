#pragma once

struct SkeletonBehaviour
{
	float updateInterval = 0.4f; // not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	const float attackStunDuration = 0.8f;
	int attackHitboxID = 2;

	float updatePathCounter = 1.8f;
	float updatePathLimit = 0.5f;

	float dirX = 0.f;
	float dirZ = 0.f;
	float dir2X = 0.f;
	float dir2Z = 0.f;
	bool followPath = false;

	int counterForTest = 0;


	float testUpdateTimer = 1.f;
	float testUpdateLimit = 0.005f;


	float fx = 0.f;
	float fz = 0.f;

	std::vector<float> coolVec;
};