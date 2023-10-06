#pragma once

struct HellhoundBehaviour
{
	float updateInterval = 0.4f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	bool clockwiseCircle = false;
	bool circleBehaviour = false;
	bool isBehind = false;
	float isBehindCounter = 0.f;
	float giveUpChaseCounter = 0.f;
	bool charge = false;
	float attackTimer = 0.f; //used as counter
	float attackStunDurationCounter = 0.f;
	float attackStunDuration = 0.8f;
	float shootingCounter = 0.f;
	float shootingDuration = 1.5f;
	bool hasShot = false;
};