#include "CollisionFunctions.h"
#include "Registry.h"
#include "Components.h"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "Level.h"
#include "UIRenderer.h"
#include <assert.h>
#include "UIRenderer.h"
#define SOFT_COLLISION_FACTOR 0.5f


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
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
	
	if (params.hitboxID1 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		radius1 = hitbox1->circleHitbox[params.hitboxID1].radius;
	}
	else //Convex (Just use bounding radius for now)
	{
		radius1 = hitbox1->convexHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
	}
	float radius2 = 0.0f;
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);
	
	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)
	{
		radius2 = hitbox2->circleHitbox[params.hitboxID2].radius;
	}
	else
	{
		radius2 = hitbox2->convexHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
	}
	float normX = params.normal1X;
	float normZ = params.normal1Z;
	//Get distance, get ratio between radi and distance
	float dist = sqrtf(normX * normX + normZ * normZ);
	float len = dist - radius1 - radius2;
	
	//Offset by ratio of mass
	float massRatio = transform1->mass / transform2->mass;

	transform1->positionX += len * normX * GetDeltaTime() * SOFT_COLLISION_FACTOR / massRatio;//Push of by
	transform1->positionZ += len * normZ * GetDeltaTime() * SOFT_COLLISION_FACTOR / massRatio;//Push of by

	//transform2->positionX += dirX * GetDeltaTime() * SOFT_COLLISION_FACTOR * massRatio;//Push of by
	//transform2->positionZ += dirZ * GetDeltaTime() * SOFT_COLLISION_FACTOR * massRatio;//Push of by

}

void HardCollision(OnCollisionParameters& params)
{
	//Take both entities transform
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(params.entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);
	assert(transform1 != nullptr && transform2 != nullptr);//No transform components added!, please add TransformComponent to your entities
	//calc dist and direction from eachother. Push out.

	float radius1 = 0.0f;
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);

	if (params.hitboxID1 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		radius1 = hitbox1->circleHitbox[params.hitboxID1].radius;
	}
	else //Convex (Just use bounding radius for now)
	{
		radius1 = hitbox1->convexHitbox[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
	}
	float radius2 = 0.0f;
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);

	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)
	{
		radius2 = hitbox2->circleHitbox[params.hitboxID2].radius;
	}
	else
	{
		radius2 = hitbox2->convexHitbox[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
	}
	float dirX, dirZ;//From 1 to 2

	dirX = params.pos2X - params.pos1X;
	dirZ = params.pos2Z - params.pos1Z;
	if (dirX == 0.0f && dirZ == 0.0f)
	{
		dirX = -transform1->positionX + 0.001f;
		dirZ = -transform1->positionZ + 0.001f;
	}
	//Get distance, get ratio between radi and distance
	float dist = sqrtf(dirX * dirX + dirZ * dirZ);
	float len = dist - radius1 - radius2;
	//
	//Offset by ratio of mass
	float massRatio = transform2->mass / transform1->mass;
	//float massRatioR = 1.0f / massRatio;//Reversed mass ratio

	transform1->positionX += dirX * len * massRatio;//Push of by
	transform1->positionZ += dirZ * len * massRatio;//Push of by

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
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
	if (params.hitboxID1 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		//Check if possible to deal damage
		if (!hitbox1->circularFlags[params.hitboxID1].canDealDamage)
		{
			return;
		}
	}
	else //For convex hitboxes
	{
		if (!hitbox1->convexFlags[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].canDealDamage)
		{
			return;
		}
	}
	//Check if hitbox2 can take damage
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);
	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		//Check if possible to deal damage
		if (!hitbox2->circularFlags[params.hitboxID2].canTakeDamage)
		{
			return;
		}
	}
	else //For convex hitboxes
	{
		if (!hitbox2->convexFlags[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].canTakeDamage)
		{
			return;
		}
	}
	//Check if hitbox already dealt damage
	for (size_t i = 0; i < HIT_TRACKER_LIMIT; i++)
	{	
		//If already in hit tracker: no proc
		if (hitbox1->hitTracker[i].active && hitbox1->hitTracker[i].entity.index == params.entity2.index)
		{
			return;
		}
	}
	//Not found, deal damage here
	stat2->health -= stat1->damage;
	RedrawUI();
	//Lastly set for hitboxTracker[]
	for (size_t i = 0; i < HIT_TRACKER_LIMIT; i++)
	{
		//If already in hit tracker: no proc
		if (!hitbox1->hitTracker[i].active)
		{
			hitbox1->hitTracker[i].active = true;
			hitbox1->hitTracker[i].entity = params.entity2;
			return;
		}
	}
}

void LoadNextLevel(OnCollisionParameters& params)
{
	LoadLevel(stateManager.activeLevelScene + 1);
}

