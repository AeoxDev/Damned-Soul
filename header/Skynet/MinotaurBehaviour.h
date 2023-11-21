#pragma once

struct MinotaurBehaviour
{
	float updateInterval = 0.2f;
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	float attackStunTimer = 0.f;
	float attackStunDuration = 2.0f;
	float chargeTimer = 0.0f;
	float facingX = 0.0f;
	float facingZ = 0.0f;
	float targetX = 0.0f;
	float targetZ = 0.0f;
	float airTimer = 0.0f;
	float airDuration = 2.5f;

	bool charging = false;		//bool to check if the minotaur is currently charging or not
	bool jumping = false;
};