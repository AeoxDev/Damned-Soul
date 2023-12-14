#pragma once

struct LuciferBehaviour
{
	float updateInterval = 0.4f; // not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;

	int hitBoxID = 2;



	// for the 10 second or damage charge
	float chargeBehevCounter = 0.f;
	float chargeBehevCounterTiming = 10.f;
	float limitHP = 0.f; //how much hp left when he stops charge

	//to handle the daze effect
	bool isDazed = false;
	float dazeCounter = 0.f;
	float dazeTimeAmount = 3.f;



	//how often pathfinding
	float updatePathCounter = 1.8f;
	float updatePathLimit = 0.5f;

	//pathfinding
	float dirX = 0.f;
	float dirZ = 0.f;
	float dir2X = 0.f;
	float dir2Z = 0.f;
	bool followPath = false;


	//trust me, these are needed. int errors
	float fx = 0.f;
	float fz = 0.f;

	//special attacks variables
	bool nextSpecialIsSpawn = false;
	bool isChargeCharge = false;
	bool isJumpJump = false;
	bool isSpawning = false;
	float spawnTimeLimit = 1.f;
	float spawnTimer = 0.f;
	int spawnIndexCounter = 0;


	// MELEE VARIABLES
	float attackTimer = 0.f; //used as counter
	float attackStunDurationCounter = 0.8f;
	const float attackStunDuration = 0.8f;
	float meleeDistance = 14.5f;
	int attackHitboxID = 3;

	// variables for jump jump 
	float flyTime = 3.25f; // Herman Was Here: det var smärtsamt hur länge han stannade i luften
	float flyCounter = 0.f;
	bool hasLandingPos = false;
	float heroLandingTiming = 0.2f;
	float heroLandingCounter = 0.f;
	bool dazedFromFly = false;

	bool isAttacking = false; // please for the love of god, DO NOT TOUCH THIS

	// damage ignore things, to balance. not touchie touchie
	float toIgnorehealthCap = 0.f;
	
};