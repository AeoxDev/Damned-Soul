#pragma once

struct EyeBehaviour
{
	float updateInterval = 0.2f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	bool clockwiseCircle = false;
	bool circleBehaviour = false;
	float giveUpChaseCounter = 0.f;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	float attackStunDuration = 0.8f;
};