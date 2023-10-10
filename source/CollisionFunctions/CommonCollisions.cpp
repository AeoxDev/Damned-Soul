#include "CollisionFunctions.h"
#include "Registry.h"
#include "Components.h"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "Level.h"
#include <assert.h>
//#include <iostream>
#define SOFT_COLLISION_FACTOR 0.4f


void NoCollision(OnCollisionParameters &params)
{
	//std::cout << "No Collision Function set on hitbox: " << params.hitboxID1 << " in entity: " << params.entity1.index << std::endl;
}

void SoftCollision(OnCollisionParameters& params)
{
	//Take both entities transform
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(params.entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);
	assert(transform1 != nullptr && transform2 != nullptr);//No transform components added!, please add TransformComponent to your entities
	//calc dist and direction from eachother. Push out.
	
	float radius1 = 0.0f;
	float offset1X = .0f;
	float offset1Z = .0f;
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
	
	if (params.hitboxID1 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		radius1 = hitbox1->circleHitbox[params.hitboxID1].radius;
		offset1X = hitbox1->circleHitbox[params.hitboxID1].offsetX;
		offset1Z = hitbox1->circleHitbox[params.hitboxID1].offsetZ;
		if (transform1 != nullptr)
		{
			offset1X += transform1->positionX;
			offset1Z += transform1->positionZ;
		}
	}
	else //Convex (Just use bounding radius for now)
	{
		radius1 = hitbox1->convexHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
		offset1X = hitbox1->circleHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].offsetX;
		offset1Z = hitbox1->circleHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].offsetZ;
		if (transform1 != nullptr)
		{
			offset1X += transform1->positionX;
			offset1Z += transform1->positionZ;
		}
	}
	float radius2 = 0.0f;
	float offset2X = .0f;
	float offset2Z = .0f;
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);
	
	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)
	{
		radius2 = hitbox2->circleHitbox[params.hitboxID2].radius;
		offset2X = hitbox2->circleHitbox[params.hitboxID2].offsetX;
		offset2Z = hitbox2->circleHitbox[params.hitboxID2].offsetZ;
		if (transform2 != nullptr)
		{
			offset2X += transform2->positionX;
			offset2Z += transform2->positionZ;
		}
	}
	else
	{
		radius2 = hitbox2->convexHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
		offset2X = hitbox2->circleHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].offsetX;
		offset2Z = hitbox2->circleHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].offsetZ;
		if (transform2 != nullptr)
		{
			offset2X += transform2->positionX;
			offset2Z += transform2->positionZ;
		}
	}
	float dirX, dirZ;//From 1 to 2

	dirX = offset2X - offset1X;
	dirZ = offset2Z - offset1Z;
	
	

	transform1->positionX -= dirX * GetDeltaTime() * SOFT_COLLISION_FACTOR;//Push of by
	transform1->positionZ -= dirZ * GetDeltaTime() * SOFT_COLLISION_FACTOR;//Push of by

	transform2->positionX += dirX * GetDeltaTime() * SOFT_COLLISION_FACTOR;//Push of by
	transform2->positionZ += dirZ * GetDeltaTime() * SOFT_COLLISION_FACTOR;//Push of by

}

void HardCollision(OnCollisionParameters& params)
{
	//Take both entities transform
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(params.entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);
	assert(transform1 != nullptr && transform2 != nullptr);//No transform components added!, please add TransformComponent to your entities
	//calc dist and direction from eachother. Push out.

	float radius1 = 0.0f;
	float offset1X = .0f;
	float offset1Z = .0f;
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);

	if (params.hitboxID1 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		radius1 = hitbox1->circleHitbox[params.hitboxID1].radius;
		offset1X = hitbox1->circleHitbox[params.hitboxID1].offsetX;
		offset1Z = hitbox1->circleHitbox[params.hitboxID1].offsetZ;
		if (transform1 != nullptr)
		{
			offset1X += transform1->positionX;
			offset1Z += transform1->positionZ;
		}
	}
	else //Convex (Just use bounding radius for now)
	{
		radius1 = hitbox1->convexHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
		offset1X = hitbox1->circleHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].offsetX;
		offset1Z = hitbox1->circleHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].offsetZ;
		if (transform1 != nullptr)
		{
			offset1X += transform1->positionX;
			offset1Z += transform1->positionZ;
		}
	}
	float radius2 = 0.0f;
	float offset2X = .0f;
	float offset2Z = .0f;
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);

	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)
	{
		radius2 = hitbox2->circleHitbox[params.hitboxID2].radius;
		offset2X = hitbox2->circleHitbox[params.hitboxID2].offsetX;
		offset2Z = hitbox2->circleHitbox[params.hitboxID2].offsetZ;
		if (transform2 != nullptr)
		{
			offset2X += transform2->positionX;
			offset2Z += transform2->positionZ;
		}
	}
	else
	{
		radius2 = hitbox2->convexHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
		offset2X = hitbox2->circleHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].offsetX;
		offset2Z = hitbox2->circleHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].offsetZ;
		if (transform2 != nullptr)
		{
			offset2X += transform2->positionX;
			offset2Z += transform2->positionZ;
		}
	}
	float dirX, dirZ;//From 1 to 2

	dirX = offset2X - offset1X;
	dirZ = offset2Z - offset1Z;

	//Get distance, get ratio between radi and distance
	float dist = sqrtf(dirX * dirX + dirZ * dirZ);
	float len = dist - radius1 - radius2;
	//
	transform1->positionX += dirX * len;//Push of by
	transform1->positionZ += dirZ * len;//Push of by

	//transform2->positionX += dirX;//Push of by
	//transform2->positionZ += dirZ;//Push of by
}

void AttackCollision(OnCollisionParameters& params)
{
	//Get the components of the attacker (entity 1)
	StatComponent* stat1 = registry.GetComponent<StatComponent>(params.entity1);

	//Get the components of the attackee (entity 2)
	StatComponent* stat2 = registry.GetComponent<StatComponent>(params.entity2);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);
	
	stat2->health -= stat1->damage;
	
}

void LoadNextLevel(OnCollisionParameters& params)
{
	LoadLevel(stateManager.activeLevelScene + 1);
}

