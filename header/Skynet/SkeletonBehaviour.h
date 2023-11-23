#pragma once


//Uncomment this line for visualizing a doggo being the path. MAKE SURE THERE IS A DOGGO WITHOUT BEHAVIOR/ COLLISION
//#define PATH_FINDING_VISUALIZER



struct SkeletonBehaviour
{
	float updateInterval = 0.4f; // not const, this is modified
	float timeCounter = 0.f;
	float goalDirectionX = 0.f, goalDirectionZ = 0.f;
	float attackTimer = 0.f;
	const float attackStunDuration = 0.8f;
	float attackStunDurationCounter = attackStunDuration;
	int attackHitboxID = 2;

	float meleeDistance = 2.5f;

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

	//pathfinding vector, debug
#ifdef PATH_FINDING_VISUALIZER
	std::vector<float> coolVec;
	float testUpdateTimer = 1.f;
	float testUpdateLimit = 0.005f;
	int counterForTest = 0;
#endif // TEST

};