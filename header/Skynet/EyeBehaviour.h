#pragma once

struct EyeBehaviour
{
	float updateInterval = 0.2f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	bool clockwiseCircle = false;
	bool circleBehaviour = false;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	float attackStunDuration = 1.4f;
	int specialCounter = 0;		//how many attacks since last special
	int specialBreakpoint = 2;	//how many attacks are needed for next special

	float facingX = 0.0f;
	float facingZ = 0.0f;
};