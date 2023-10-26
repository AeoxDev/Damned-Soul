#pragma once

struct HellhoundBehaviour
{
	// MOVEMENT VARIABLES
	float updateInterval = 0.4f; //not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	bool clockwiseCircle = false;
	bool circleBehaviour = false;
	bool isBehind = false;
	float isBehindCounter = 0.f;
	float giveUpChaseCounter = 0.f;
	bool charge = false;
	




	// MELEE VARIABLES
	float attackTimer = 0.f; //used as counter
	float attackStunDurationCounter = 0.8f;
	const float attackStunDuration = 0.8f;




	// SHOOTING VARIABLES

	float shootingCounter = 0.f;
	const float shootingChargeUpTime = 1.1f; //how long it takes to shoot
	bool isShooting = false;
	const float shootingCooldown = 10.0f; //seconds. ability cooldown
	float shootingCooldownCounter = shootingCooldown; // so it can start shooting from start, it gets changed to 0 in code
	float shootingMaxTime = 2.0f;//Stop shooting after this amount of seconds.
	float shootingTimer = 0.0f;//The current time for shooting
	//----------------
	float shootingStartX = 0.f;
	float shootingStartZ = 0.f;
	//float maxShootingAttackRange = 0.0f;//used to determine max range in an attack
	float shootingSideTarget1X = 0.f;
	float shootingSideTarget1Z = 0.f;
	float shootingSideTarget2X = 0.f;//these are used for creating a hitbox triangle
	float shootingSideTarget2Z = 0.f;
	const float offsetSide = 10.f; // used to get the extra 2 points for the triangle in shooting
	const float offsetForward = 30.f; // used to aim a little behind player, so it's not super easy to dodge
	//---------------------
	float facingX = 0.f;
	float facingZ = 0.f;
	const float shootingAttackSpeedForHitbox = 20.f; //change as you see fit
	float currentShootingAttackRange = 0.0f; // used as a counter 
	float flameDamage = 0.08f; // flame damage per frame
	//------------
	bool flameSoundsStartedPlaying = false;
	


	// COWARD VARIABLES
	const float cowardDuration = 2.f; // How long the dog will run away
	float cowardCounter = 0.f;
	float lastPositionX = 0.f;// These 2 positions are used to determine which
	float lastPositionZ = 0.f;// way to run, minimizing the risk of running into 
							  // a wall
	float cowardDirectionX = 0.f; // the direction pos give us
	float cowardDirectionZ = 0.f;
	bool retreat = false;
	bool isWating = false; // kind of dumb, but needed for checks


	
};