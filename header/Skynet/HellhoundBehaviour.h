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
	float shootingDuration = 1.5f; //how long it takes to shoot
	bool isShooting = false;
	float shootingCooldown = 4.0f; //seconds. ability cooldown
	float shootingCooldownCounter = 0.f;
	float shootingTargetX = 0.f;
	float shootingTargetZ = 0.f;
	float shootingAttackSpeedForHitbox = 10.f; //change as you see fit
	float currentShootingAttackRange = 0.0f; // used as a counter 
	float maxShootingAttackRange = 5.0f;//change as you see fit
};