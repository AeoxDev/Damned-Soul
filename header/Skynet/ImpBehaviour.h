#pragma once

struct ImpBehaviour
{
	float chaseTimer = 2.5f;
	float chaseCounter = 0.f; 
	float idleTimer = 4.0f;
	float idleCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.0f;
	float attackStunDuration = 2.0f;
	float attackStunTimer = attackStunDuration;
	float aimTimer = 0.0f;
	float aimDuration = 0.7f;
	float maxAttackRange = 50.f;
	
	int specialCounter = 0;		//how many attacks since last special
	int specialBreakpoint = 4;	//how many attacks are needed for next special

	bool clockwiseCircle = true;
	bool circleBehaviour = false;
	bool charging = false;		//bool to check if the eye is currently charging at the players position or not
	bool shooting = false;
	bool dealtDamage = false;
	bool updateIdleTimer = false;
};