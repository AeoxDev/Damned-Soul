#pragma once

struct EyeBehaviour
{
	float updateInterval = 0.2f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunDurationCounter = 0.f;
	float attackStunDuration = 2.0f;
	float chargeTimer = 0.0f;
	float aimTimer = 0.0f;
	float aimDuration = 0.4f;
	float facingX = 0.0f;
	float facingZ = 0.0f;
	float targetX = 0.0f;
	float targetZ = 0.0f;
	float changeDirX = 0.0f;
	float changeDirZ = 0.0f;


	int specialCounter = 0;		//how many attacks since last special
	int specialBreakpoint = 4;	//how many attacks are needed for next special

	bool clockwiseCircle = true;
	bool circleBehaviour = false;
	bool charging = false;		//bool to check if the eye is currently charging at the players position or not
	bool dealtDamage = false;
	bool chargeAttackSoundPlaying = false;
};