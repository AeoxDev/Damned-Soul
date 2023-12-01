#include "Hitbox.h"
#include "D3D11Helper\D3D11Graphics.h"
#include "D3D11Helper\D3D11Helper.h"
#include <fstream>
#include <sstream>
#include "Backend/Collision.h"
#include "CollisionFunctions.h"
#include "Backend\GeometryIndependent.h"
#include "Components.h"
#include "GameRenderer.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Skynet\BehaviourHelper.h"

int CreateHitbox(EntityID& entity, float radius, float offsetX, float offsetZ)
{
	//Check for the HitboxComponent, return -1 if failed
	HitboxComponent* collisionComponent = registry.GetComponent<HitboxComponent>(entity);
	if (collisionComponent == nullptr)
	{
		//Component did not exist!
		return -1;
	}
	//We now have the Collision Component, look for the first bit to add a hitbox on
	unsigned availableSlot = FindAvailableSlot(collisionComponent->usedCirclesHitboxes);
	if (availableSlot < 0)
	{
		//No available bits, maximum size achieved
		return -2;
	}
	//There is now a slot for the hitbox, create it on the given slot.
	collisionComponent->circleHitbox[availableSlot].radius = radius;
	collisionComponent->circleHitbox[availableSlot].offsetX = offsetX;
	collisionComponent->circleHitbox[availableSlot].offsetZ = offsetZ;
	//Set to mode
	collisionComponent->circularFlags[availableSlot].ResetToActive();
	//Look at components to find what bit flags should be used
	SetCollisionEvent(entity, (int)availableSlot, NoCollision );

	//Visualize hitbox if possible
#ifdef _DEBUG
	if (hitboxVisualizerActive[availableSlot])
	{
		VisualizeHitbox(entity, availableSlot);
	}
#endif // _DEBUG


	//Return the chosen slot for the user for further uses.
	
	return availableSlot;
}

void RemoveHitbox(EntityID& entity, int hitboxID)
{
	//Set slot flag to 0
	unsigned mask = 0b1;//0b00000000 00000000 00000000 00000001
	// ob0011
	// ob0101
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}

	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		//Find slot flag
		hitbox->usedCirclesHitboxes = hitbox->usedCirclesHitboxes & (~(mask << hitboxID));
		//Set collision flags to 0
		hitbox->circularFlags[hitboxID].ResetToActive();
		hitbox->circularFlags[hitboxID].active = 0;
	}
	else
	{
		int convexSlot = hitboxID - SAME_TYPE_HITBOX_LIMIT;
		//Find slot flag
		hitbox->usedConvexHitboxes = hitbox->usedConvexHitboxes & (~(mask << (convexSlot)));
		//Set collision flags to 0
		hitbox->convexFlags[convexSlot].ResetToActive();
		hitbox->convexFlags[convexSlot].active = 0;
	}
}



int CreateHitbox (EntityID& entity, int corners, float cornerPosX[], float cornerPosZ[])
{
	if (corners < 3 && corners >= CONVEX_CORNER_LIMIT)
	{
		return -3;
	}
	//Check for the HitboxComponent, return -1 if failed
	HitboxComponent* collisionComponent = registry.GetComponent<HitboxComponent>(entity);
	if (collisionComponent == nullptr)
	{
		//Component did not exist!
		return -1;
	}
	//We now have the Collision Component, look for the first bit to add a hitbox on
	unsigned availableSlot = FindAvailableSlot(collisionComponent->usedConvexHitboxes);
	if (availableSlot < 0)
	{
		//No available bits, maximum size achieved
		return -2;
	}
	//There is now a slot for the hitbox, create it on the given slot.
	/*collisionComponent->circleHitbox[availableSlot].radius = radius;
	collisionComponent->circleHitbox[availableSlot].offsetX = offsetX;
	collisionComponent->circleHitbox[availableSlot].offsetZ = offsetZ;*/
	float sumX = 0.f;
	float sumZ = 0.f;
	for (int i = 0; i < corners; i++)
	{
		collisionComponent->convexHitbox[availableSlot].cornerX[i] = cornerPosX[i];
		sumX += cornerPosX[i];

		collisionComponent->convexHitbox[availableSlot].cornerZ[i] = cornerPosZ[i];
		sumZ += cornerPosZ[i];
	}
	collisionComponent->convexHitbox[availableSlot].cornerAmount = corners;
	//Calculate centroid
	collisionComponent->convexHitbox[availableSlot].centerX = (sumX / (float)corners);
	collisionComponent->convexHitbox[availableSlot].centerZ = (sumZ / (float)corners);
	//Calculate boundingRadius
	//Get longest line from centroid
	float longestDistance = 0.f;
	for (int i = 0; i < corners; i++)
	{
		float dx, dz = 0.f;
		dx = cornerPosX[i] - collisionComponent->convexHitbox[availableSlot].centerX;
		dx = dx * dx;

		dz = cornerPosZ[i] - collisionComponent->convexHitbox[availableSlot].centerZ;
		dz = dz * dz;

		float tempDist = dx * dx + dz * dz;

		if (tempDist > longestDistance)
		{
			longestDistance = tempDist;
		}
	}
	collisionComponent->convexHitbox[availableSlot].boundingRadius = std::sqrt(longestDistance);
	
	//Set normals
	float lineX, lineZ = 0.f;
	float scalar = 0.f;
	for (int i = 0; i < corners; i++)
	{
		//First get line
		lineX = cornerPosX[(i + 1) % corners] - cornerPosX[i];
		lineZ = cornerPosZ[(i + 1) % corners] - cornerPosZ[i];
		//Rotate 90 degrees for normal
		collisionComponent->convexHitbox[availableSlot].normalX[i] = -lineZ;
		collisionComponent->convexHitbox[availableSlot].normalZ[i] = lineX;
		//Get line from centroid to corner
		lineX = cornerPosX[i] - collisionComponent->convexHitbox[availableSlot].centerX;
		lineZ = cornerPosZ[i] - collisionComponent->convexHitbox[availableSlot].centerZ;
		//Check if scalar is positive, if negative, reverse normal direction
		scalar = (lineX * collisionComponent->convexHitbox[availableSlot].normalX[i]) + (lineZ * collisionComponent->convexHitbox[availableSlot].normalZ[i]);
		if (scalar <= 0.f)
		{
			collisionComponent->convexHitbox[availableSlot].normalX[i] *= -1.0f;
			collisionComponent->convexHitbox[availableSlot].normalZ[i] *= -1.0f;
		}
		float magnitude = sqrt(collisionComponent->convexHitbox[availableSlot].normalX[i] * collisionComponent->convexHitbox[availableSlot].normalX[i] +
			collisionComponent->convexHitbox[availableSlot].normalZ[i] * collisionComponent->convexHitbox[availableSlot].normalZ[i]); // very logical line, yes.
		collisionComponent->convexHitbox[availableSlot].normalX[i] /= magnitude;
		collisionComponent->convexHitbox[availableSlot].normalZ[i] /= magnitude;
	}

	float line2X, line2Z = 0.f;
	float magnitudeX, magnitudeZ = 0.f;
	float radians = 0.f;
	bool reverse = false;
	////Check if convex
	//for (int i = 0; i < corners; i++)
	//{
	//	//First get line
	//	lineX = cornerPosX[(i + 1) % corners] - cornerPosX[i];
	//	lineZ = cornerPosZ[(i + 1) % corners] - cornerPosZ[i];

	//	//Second line
	//	line2X = cornerPosX[(i + 2) % corners] - cornerPosX[(i + 1) % corners];
	//	line2Z = cornerPosZ[(i + 2) % corners] - cornerPosZ[(i + 1) % corners];

	//	// dot product
	//	scalar = (lineX * line2X) + (lineZ * line2Z);

	//	//calculate magnitude of the lnes
	//	magnitudeX = std::sqrt(lineX * lineX + lineZ * lineZ);
	//	magnitudeZ = std::sqrt(line2X * line2X + line2Z * line2Z);

	//	//angle in radians
	//	radians = std::acos(scalar / (magnitudeX * magnitudeZ));

	//	if (i == 0 && radians - 0.0001f> 3.14159265359f * 0.5f)
	//	{
	//		reverse = true;
	//	}
	//	if (reverse)
	//	{
	//		radians = 3.14159265359f - radians;
	//	}
	//	if (radians > 3.14159265359f * 0.5f)
	//	{
	//		//Concave shape, sadge
	//		RemoveHitbox(entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
	//		return -4;
	//	}
	//	//Check if middle point of neighboring points is on the inside
	//	//First get middle point
	//	float middleX = (cornerPosX[i] + cornerPosX[(i + 1) % corners] + cornerPosX[(i + 2) % corners]) * 0.333f;
	//	float middleZ = (cornerPosZ[i] +cornerPosZ[(i + 1) % corners] + cornerPosZ[(i + 2) % corners]) * 0.333f;
	//	float middleToCorner1X = cornerPosX[i] - middleX;
	//	float middleToCorner1Z = cornerPosZ[i] - middleZ;
	//	float middleToCorner2X = cornerPosX[(i + 2) % corners] - middleX;
	//	float middleToCorner2Z = cornerPosZ[(i + 2) % corners] - middleZ;
	//	//Then do scalar product to verify direction
	//	scalar = (middleToCorner1X * collisionComponent->convexHitbox[availableSlot].normalX[i]) + (middleToCorner1Z * collisionComponent->convexHitbox[availableSlot].normalZ[i]);
	//	if (scalar < 0.0f)
	//	{
	//		RemoveHitbox(entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
	//		return -5;
	//	}
	//	scalar = (middleToCorner2X * collisionComponent->convexHitbox[availableSlot].normalX[(i + 1) % corners]) + (middleToCorner2Z * collisionComponent->convexHitbox[availableSlot].normalZ[(i + 1) % corners]);
	//	if (scalar < 0.0f)
	//	{
	//		RemoveHitbox(entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
	//		return -5;
	//	}
	//}

	//Set to mode
	collisionComponent->convexFlags[availableSlot].ResetToActive();
	//Look at components to find what bit flags should be used
	SetCollisionEvent(entity, (int)availableSlot + SAME_TYPE_HITBOX_LIMIT, NoCollision);
	//Visualize hitbox if possible
#ifdef _DEBUG
	if (hitboxVisualizerActive[availableSlot + SAME_TYPE_HITBOX_LIMIT])
	{
		VisualizeHitbox(entity, availableSlot +SAME_TYPE_HITBOX_LIMIT);
	}
#endif // _DEBUG
	//Return the chosen slot for the user for further uses.
	return availableSlot + SAME_TYPE_HITBOX_LIMIT;
}

void AddHitboxComponent(EntityID& entity)
{
	registry.AddComponent<HitboxComponent>(entity);
}

void CreateProximityHitbox(EntityID& entity, std::string fileName)
{
	//Check if the proximityComponent already exists.
	ProximityHitboxComponent* proximityComponent = registry.GetComponent<ProximityHitboxComponent>(entity);
	if (proximityComponent == nullptr)
	{
		//Component did not exist!
		return;
	}

	//Read file and fill up the pointList and tell if clockwise or counter-clockwise
	std::string filePath = "HitboxFiles/" + fileName + ".box";
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return; //File not found
	}

	std::string line;
	ProximityPoint currentPoint;

	while (std::getline(file, line)) //For each line in the file
	{
		std::stringstream ss(line); //Load line into a stringstream

		std::string word;

		ss >> word; //Prefix

		if (word == "Clockwise") //Set clockwise variable to the value read from file
		{
			ss >> word;
			proximityComponent->clockwise = std::stoi(word);
		}
		else if (word == "Point") //Start of a new point, add index to ProximityPoint
		{
			ss >> word;
			currentPoint.index = std::stoi(word);
		}
		else if (word == "X") //X coordinate for the ProximityPoint
		{
			ss >> word;
			currentPoint.x = (float)(std::stoi(word));
		}
		else if (word == "Z") //Z coordinate for the ProximityPoint, also marks a completed point to add to component list.
		{
			ss >> word;
			currentPoint.z = (float)(std::stoi(word));

			proximityComponent->pointList.push_back(currentPoint);
		}
	}
}

void AddProximityHitboxComponent(EntityID& entity)
{
	registry.AddComponent<ProximityHitboxComponent>(entity);
}

void SetHitboxActive(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].active = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].active = setFlag;
	}
	//ResetAttackTrackerFlags(entity);
}

void SetHitboxIsStage(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isStage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isStage = setFlag;
	}
}

void SetHitboxIsWall(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isWall = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isWall = setFlag;
	}
}

void SetHitboxIsPlayer(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent *hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isPlayer = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].isPlayer = setFlag;
	}
}

void SetHitboxIsEnemy(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isEnemy = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].isEnemy = setFlag;
	}
}

void SetHitboxIsStaticHazard(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isStaticHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isStaticHazard = setFlag;
	}
}

void SetHitboxIsDynamicHazard(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isDynamicHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isDynamicHazard = setFlag;
	}
}

void SetHitboxIsMoveable(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isMoveable = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isMoveable = setFlag;
	}
}

void SetHitboxCanTakeDamage(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].canTakeDamage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].canTakeDamage = setFlag;
	}
	//ResetAttackTrackerFlags(entity);
}

void SetHitboxCanDealDamage(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (!GetHitboxCanDealDamage(entity, hitboxID) && setFlag == true)
	{
		ResetAttackTrackerFlags(entity);
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].canDealDamage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].canDealDamage = setFlag;
	}
}

void SetHitboxHitStage(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitStage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitStage = setFlag;
	}
}

void SetHitboxHitWall(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitWall = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitWall = setFlag;
	}
}

void SetHitboxHitPlayer(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitPlayer = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].hitPlayer = setFlag;
	}
}

void SetHitboxHitEnemy(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitEnemy = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].hitEnemy = setFlag;
	}
}

void SetHitboxHitStaticHazard(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitStaticHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitStaticHazard = setFlag;
	}
}

void SetHitboxHitDynamicHazard(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr)
	{
		return;
	}
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitDynamicHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitDynamicHazard = setFlag;
	}
}

void SetupEnemyCollisionBox(EntityID& entity, float radius, EnemyType etype, bool affectedByStaticHazards)
{
	AddHitboxComponent(entity);
	EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(entity);
	TransformComponent* transfor = registry.GetComponent<TransformComponent>(entity);
	int hID = CreateHitbox(entity, radius*0.2f, 0.f, 0.f);
	SetCollisionEvent(entity, hID, HardCollision);
	SetHitboxIsEnemy(entity, hID);
	SetHitboxHitPlayer(entity, hID);
	SetHitboxHitEnemy(entity, hID);
	SetHitboxActive(entity, hID);
	SetHitboxIsMoveable(entity, hID);
	//SetHitboxHitWall(entity, hID);

	int sID = CreateHitbox(entity, radius, 0.f, 0.f);
	SetCollisionEvent(entity, sID, SoftCollision);
	SetHitboxIsEnemy(entity, sID);
	SetHitboxHitPlayer(entity, sID);
	//SetHitboxHitEnemy(entity, sID);
	SetHitboxActive(entity, sID);
	SetHitboxIsMoveable(entity, sID);
	SetHitboxHitStaticHazard(entity, sID, affectedByStaticHazards);
	SetHitboxCanTakeDamage(entity, sID);

	SetHitboxCanDealDamage(entity, sID, false);


	switch (etype)
	{
	case EnemyType::eye:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 1.2f, 0.f, radius * 1.0f);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);
		break;

	case EnemyType::imp:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 1.0f, 0.f, radius * 1.0f);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);
		break;

	case EnemyType::hellhound:
		//Regular attack hitbox

		/*Player attack hitbox for reference, wider at the base
		corners.cornersX[0] = -width;
		corners.cornersX[1] = width;
		corners.cornersX[2] = 2.0f * width;
		corners.cornersX[3] = -2.0 * width;
		// Z
		corners.cornersZ[0] = -2.f * depth;
		corners.cornersZ[1] = -2.f * depth;
		corners.cornersZ[2] = -0.5f;
		corners.cornersZ[3] = -0.5f;
		*/



		{
			float cornersX[4];		//Width
			cornersX[0] = -2.25f;	//Thinner at the tip
			cornersX[1] = 2.25f;	//
			cornersX[2] = 3.0f;		//Thicker at the base
			cornersX[3] = -3.0f;	//
			float cornersZ[4];		//Length
			cornersZ[0] = -7.5f;	//Long reach forwards
			cornersZ[1] = -7.5f;	//
			cornersZ[2] = 1.0f;		//Slightly offset backwards
			cornersZ[3] = 1.0f;		//
			enemyComp->attackHitBoxID = CreateHitbox(entity, 4, cornersX, cornersZ);
			//enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 1.5f, 0.f, radius * -2.0f);
		}
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, true);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);

		
		//Breath attack hitbox (cone)
		float cornersX[3];// = { 0.0f, 0.5f, -0.5f };
		cornersX[0] = 0.0f;
		cornersX[1] = 0.5f;
		cornersX[2] = -0.5f;
		float cornersZ[3];// = { 0.0f, -0.5f, -0.5f };
		cornersZ[0] = -1.0f;
		cornersZ[1] = -1.5f;
		cornersZ[2] = -1.5f;
		enemyComp->specialHitBoxID = CreateHitbox(entity, 3, cornersX, cornersZ);
		SetCollisionEvent(entity, enemyComp->specialHitBoxID, HellhoundBreathAttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->specialHitBoxID);
		SetHitboxActive(entity, enemyComp->specialHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->specialHitBoxID, false);
		break;
	case EnemyType::minotaur:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 3.f, 0.f, radius * -4.5f);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);

		//Create special hitbox
		enemyComp->specialHitBoxID = CreateHitbox(entity, radius * 1.5f, 0.f, 0.f);
		SetCollisionEvent(entity, enemyComp->specialHitBoxID, ShockWaveAttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->specialHitBoxID);
		//SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->specialHitBoxID, false);
		break;

	case EnemyType::lucifer:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 3.0f, 0.f, radius * -4.2f);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);

		//Create special hitbox
		enemyComp->specialHitBoxID = CreateHitbox(entity, radius * 1.5f * transfor->scaleX, 0.f, 0.f);
		SetCollisionEvent(entity, enemyComp->specialHitBoxID, ShockWaveAttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->specialHitBoxID);
		//SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->specialHitBoxID, false);
		break;
	case EnemyType::tempBoss:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 1.f * transfor->scaleX, 0.f, radius * -1.0f * transfor->scaleX);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);

		//Create special hitbox
		enemyComp->specialHitBoxID = CreateHitbox(entity, radius * 1.5f * transfor->scaleX, 0.f, 0.f);
		SetCollisionEvent(entity, enemyComp->specialHitBoxID, ShockWaveAttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->specialHitBoxID);
		//SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->specialHitBoxID, false);
		break;
	case EnemyType::zac:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 2.5f, 0.f * transfor->scaleX, radius * -1.5f * transfor->scaleX);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);
		SetHitboxIsMoveable(entity, hID, false);
		SetHitboxIsMoveable(entity, sID, false);
		break;
	case EnemyType::ghost:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 2.5f, 0.f * transfor->scaleX, radius * -1.5f * transfor->scaleX);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);
		SetHitboxIsMoveable(entity, hID, false);
		SetHitboxIsMoveable(entity, sID, false);
		SetHitboxActive(entity, 0, false);
		SetHitboxActive(entity, 1, true);
		SetHitboxActive(entity, 2, false);
	default:
		enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 2.5f, 0.f * transfor->scaleX, radius * -2.0f * transfor->scaleX);
		SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
		//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
		SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
		SetHitboxActive(entity, enemyComp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);
		break;
	}
	
}


void SetupLavaCollisionBox(EntityID& entity, float radius)
{
	AddHitboxComponent(entity);
	int staticID = CreateHitbox(entity, radius, 0.f, 0.0f);
	SetCollisionEvent(entity, staticID, HellhoundBreathAttackCollision);
	//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
	SetHitboxHitPlayer(entity, staticID);
	SetHitboxActive(entity, staticID, false);
	//SetHitboxIsStaticHazard(entity, staticID);
	//SetHitboxCanTakeDamage(entity, staticID, false);
	SetHitboxCanDealDamage(entity, staticID, false);
}

void SetupProjectileCollisionBox(EntityID& entity, float radius)
{
	AddHitboxComponent(entity);
	EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(entity);
	//int hID = CreateHitbox(entity, radius * 0.5f, 0.f, 0.f);
	//SetCollisionEvent(entity, hID, HardCollision);
	//SetHitboxHitPlayer(entity, hID);
	//SetHitboxActive(entity, hID);
	//SetHitboxIsMoveable(entity, hID);

	int sID = CreateHitbox(entity, radius, 0.f, 0.f);
	SetCollisionEvent(entity, sID, ProjectileAttackCollision);
	SetHitboxHitPlayer(entity, sID);
	SetHitboxActive(entity, sID);
	SetHitboxIsMoveable(entity, sID);
	//SetHitboxCanTakeDamage(entity, sID);

	SetHitboxCanDealDamage(entity, sID, true);
	
}

bool GetHitboxCanDealDamage(EntityID& entity, int hitboxID)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		return hitbox->circularFlags[hitboxID].canDealDamage == 1;
	}
	else
	{
		return hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].canDealDamage == 1;
	}
}

void SetupPlayerCollisionBox(EntityID& entity, float radius)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);

	AddHitboxComponent(entity);

	int hID = CreateHitbox(entity, radius * 0.5f, 0.f, -0.f);
	SetCollisionEvent(entity, hID, HardCollision);
	SetHitboxIsPlayer(entity, hID);
	SetHitboxHitWall(entity, hID);
	SetHitboxHitEnemy(entity, hID);
	SetHitboxHitStage(entity, hID);
	SetHitboxActive(entity, hID);
	SetHitboxIsMoveable(entity, hID);

	/*float cornersX[] = { -0.2f, 0.2f, 0.2f, -0.2f };
	float cornersZ[] = { -4.0f, -4.0f, 2.0f, 2.0f };
	int hID = CreateHitbox(entity, 4, cornersX, cornersZ);
	SetCollisionEvent(entity, hID, HardCollision);
	SetHitboxIsPlayer(entity, hID);
	SetHitboxHitEnemy(entity, hID);
	SetHitboxHitStage(entity, hID);
	SetHitboxActive(entity, hID);
	SetHitboxIsMoveable(entity, hID);*/

	int sID = CreateHitbox(entity, radius * 0.75f, .0f, -0.0f);
	SetCollisionEvent(entity, sID, SoftCollision);
	SetHitboxIsPlayer(entity, sID, true);
	SetHitboxHitEnemy(entity, sID);
	SetHitboxActive(entity, sID);
	SetHitboxIsMoveable(entity, sID);
	SetHitboxCanTakeDamage(entity, sID, true);
	SetHitboxHitStaticHazard(entity, sID, true);
	SetHitboxHitStage(entity, sID);
	playerComp->softHitboxID = sID;
	VisualizeHitbox(entity, sID);

	// Attack
	//playerComp->attackHitboxID = CreateHitbox(entity, radius * 2.f, 0.f, -0.5f);
	{
		float cornersX[] = { -.1f,.1f,.1f,-.1f };
		float cornersZ[] = { -.1f,-.1f,.1f,.1f };
		int cornerCount = 4;
		playerComp->attackHitboxID = CreateHitbox(entity, cornerCount, cornersX, cornersZ);
	}
	SetCollisionEvent(entity, playerComp->attackHitboxID, AttackCollision);
	SetHitboxHitEnemy(entity, playerComp->attackHitboxID);

	// Dash hitbox
	playerComp->dashHitboxID = CreateHitbox(entity, radius * 2.0f, 0.0f, 0.0f);
	SetCollisionEvent(entity, playerComp->dashHitboxID, DashCollision);
	SetHitboxHitEnemy(entity, playerComp->dashHitboxID);
	SetHitboxActive(entity, playerComp->dashHitboxID);
	SetHitboxCanTakeDamage(entity, playerComp->dashHitboxID, false);
	SetHitboxCanDealDamage(entity, playerComp->dashHitboxID, false);
}

bool HitboxCanHitGI(EntityID& entity)
{
	HitboxComponent* h = registry.GetComponent<HitboxComponent>(entity);
	if (h == nullptr)
	{
		return false;
	}
	if ((h->circularFlags[0].isMoveable && h->circularFlags[0].active) || (h->convexFlags[0].isMoveable && h->convexFlags[0].active))
	{
		return true;
	}
	return false;
}

void SetCollisionEvent(EntityID& entity, int hitboxID, void* function)
{
	//Find hitbox
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);

	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->onCircleCollision[hitboxID].CollisionFunction = (void(*)(OnCollisionParameters&))function;
	}
	else
	{
		hitbox->onConvexCollision[hitboxID- SAME_TYPE_HITBOX_LIMIT].CollisionFunction = (void(*)(OnCollisionParameters&))function;
	}
	
}

ConvexReturnCorners GetHitboxCorners(EntityID& entity, int hitboxID)
{
	//Find hitbox
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitbox == nullptr || hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		return { -1, nullptr, nullptr };
	}
	else
	{
		auto data = hitbox->convexHitbox[hitboxID - SAME_TYPE_HITBOX_LIMIT];
		return { data.cornerAmount, data.cornerX, data.cornerZ };
	}
}

void SetHitboxCorners(EntityID& entity, int hitboxID, int corners, float cornersX[], float cornersZ[])
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);

	assert(hitboxID >= SAME_TYPE_HITBOX_LIMIT);
	int slot = hitboxID - SAME_TYPE_HITBOX_LIMIT;
	//Redo center and bounding radius
	float sumX = 0.f;
	float sumZ = 0.f;
	for (int i = 0; i < corners; i++)
	{
		hitbox->convexHitbox[slot].cornerX[i] = cornersX[i];
		hitbox->convexHitbox[slot].cornerZ[i] = cornersZ[i];
		sumX += cornersX[i];
		sumZ += cornersZ[i];
	}
	hitbox->convexHitbox[slot].cornerAmount = corners;
	//Calculate centroid
	hitbox->convexHitbox[slot].centerX = (sumX / (float)corners);
	hitbox->convexHitbox[slot].centerZ = (sumZ / (float)corners);
	//Calculate boundingRadius
	//Get longest line from centroid
	float longestDistance = 0.f;
	for (int i = 0; i < corners; i++)
	{
		float dx, dz = 0.f;
		dx = cornersX[i] - hitbox->convexHitbox[slot].centerX;
		dx = dx * dx;

		dz = cornersZ[i] - hitbox->convexHitbox[slot].centerZ;
		dz = dz * dz;

		float tempDist = dx * dx + dz * dz;

		if (tempDist > longestDistance)
		{
			longestDistance = tempDist;
		}
	}
	hitbox->convexHitbox[slot].boundingRadius = sqrtf(longestDistance);
	//Redo normals
	//Set normals
	float lineX, lineZ = 0.f;
	float scalar = 0.f;
	for (int i = 0; i < corners; i++)
	{
		//First get line
		lineX = cornersX[(i + 1) % corners] - cornersX[i];
		lineZ = cornersZ[(i + 1) % corners] - cornersZ[i];
		//Rotate 90 degrees for normal
		hitbox->convexHitbox[slot].normalX[i] = -lineZ;
		hitbox->convexHitbox[slot].normalZ[i] = lineX;
		//Get line from centroid to corner
		lineX = cornersX[i] - hitbox->convexHitbox[slot].centerX;
		lineZ = cornersZ[i] - hitbox->convexHitbox[slot].centerZ;
		//Check if scalar is positive, if negative, reverse normal direction
		scalar = (lineX * hitbox->convexHitbox[slot].normalX[i]) + (lineZ * hitbox->convexHitbox[slot].normalZ[i]);
		if (scalar <= 0.f)
		{
			hitbox->convexHitbox[slot].normalX[i] *= -1.0f;
			hitbox->convexHitbox[slot].normalZ[i] *= -1.0f;
		}
		float magnitude = sqrt(hitbox->convexHitbox[slot].normalX[i] * hitbox->convexHitbox[slot].normalX[i] +
			hitbox->convexHitbox[slot].normalZ[i] * hitbox->convexHitbox[slot].normalZ[i]); // very logical line, yes.
		hitbox->convexHitbox[slot].normalX[i] /= magnitude;
		hitbox->convexHitbox[slot].normalZ[i] /= magnitude;
	}
}

void ResetAttackTrackerFlags(EntityID& entity)
{
	//If hitboxID = -1 reset all trackers
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);

	for (size_t i = 0; i < HIT_TRACKER_LIMIT; i++)
	{
		hitbox->hitTracker[i].active = false;
	}
}


void RenderGeometryIndependentCollision(EntityID& stageFloor, EntityID& gate, EntityID& hitbox)
{
	AddGeometryIndependentComponent(stageFloor);
	GeometryIndependentColliderComponent* GeoIndie = registry.GetComponent<GeometryIndependentColliderComponent>(stageFloor);
	//while (true)//Debugging GI
	//{
	//	//char ctexture[] = "2-1C.png";
	//	//char ltexture[] = "2-1L.png";
	//	//char emptyTexture[] = "";
	//	//AddStaticHazardTexture(m, ctexture, ltexture, emptyTexture);
	//	RenderGeometryIndependentCollisionToTexture(stageFloor, gate, hitbox);
	//	Present();
	//}

	RenderGeometryIndependentCollisionToTexture(stageFloor, gate, hitbox);
	ReleaseGI(stageFloor);
	return;
}
