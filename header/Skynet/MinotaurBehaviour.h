#pragma once

struct MinotaurBehaviour
{
	float updateInterval = 0.2f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunDuration = 1.5f;
	float attackStunTimer = attackStunDuration;
	float chargeTimer = 0.0f;
	float chargeDuration = 2.0f;
	float facingX = 0.0f;
	float facingZ = 0.0f;
	float targetX = 0.0f;
	float targetZ = 0.0f;

	//current direction the minotaur is charging
	float chargeDirX = 0.0f;
	float chargeDirZ = 0.0f;
	
	//used for determening how long the minotaur has been in the air and for how long he should be in the air
	float airTimer = 0.0f;
	float airDuration = 2.5f;

	//used for determening the window before the minotaur charges the player
	float jumpTimer = 0.0f;
	float JumpDuration = 0.5f;
	
	float aimTimer = 0.0f;
	float aimDuration = 1.4f;

	bool charging = false;		//bool to check if the minotaur is currently charging or not
	bool jumping = false;
	bool hasLandingPos = false;
	bool hasChargingDir = false;
	bool chargeAttackSoundPlaying = false;
};