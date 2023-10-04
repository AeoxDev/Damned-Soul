#pragma once


struct SkeletonBehaviour
{
	float updateInterval = 0.4f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	float attackStunDuration = 0.8f;
};