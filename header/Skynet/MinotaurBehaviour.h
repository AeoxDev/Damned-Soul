#pragma once

struct MinotaurBehaviour
{
	float updateInterval = 0.2f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float facingX = 0.0f;
	float facingZ = 0.0f;
	float targetX = 0.0f;
	float targetZ = 0.0f;

	int behaviourState = 0; // behaviourState is used to derermine if he should charge or jump, 0 = charge, 1 = jump

	float chargeTimer = 0.0f;
	float chargeDuration = 1.5f;
	
	//Dazed counter and duration
	float attackStunDuration = 1.8f;
	float attackStunTimer = attackStunDuration;

	//current direction the minotaur is charging
	float chargeDirX = 0.0f;
	float chargeDirZ = 0.0f;

	//used to telegraph a jump is coming
	float jumpBuildUpDuration = 0.4f;
	float jumpBuildUpTimer = 0.0f;

	float jumpStunDuration = 2.0f;
	float jumpStunTimer = jumpStunDuration;

	//used for determening how long the minotaur has been in the air and for how long he should be in the air
	float jumpTimer = 0.0f;
	float jumpDuration = 1.5f;
	
	//used to determine how many jump the minotaur has done and when to stop jumping
	int jumpCounter = 0;
	int jumpBreakpoint = 3;

	//used for determening the window before the minotaur charges the player
	float aimDuration = 1.4f;
	float aimTimer = 0.0f;

	//pathfinding
	float updatePathCounter = 1.8f;
	float updatePathLimit = 0.5f;
	float dirX = 0.f;
	float dirZ = 0.f;
	float dir2X = 0.f;
	float dir2Z = 0.f;

	//trust me, these are needed. int errors
	float fx = 0.f;
	float fz = 0.f;

	bool followPath = false;
	bool charging = false;		//bool to check if the minotaur is currently charging or not
	bool jumping = false;
	bool hasLandingPos = false;
	bool hasChargingDir = false;
	bool hasSeenPlayer = false;
	bool chargeAttackSoundPlaying = false;
};