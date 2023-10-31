#include "CollisionFunctions.h"
#include "Components.h"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "Level.h"
#include "UIRenderer.h"
#include <assert.h>
#include "RelicFunctions.h"
#include "Relics\RelicFuncInputTypes.h"
#include "EventFunctions.h"
#include "Levels/LevelHelper.h"

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

//Check if attacker is static hazard and defender can hit static hazard.
void StaticHazardAttackCollision(OnCollisionParameters& params)
{
	StatComponent* stat1 = registry.GetComponent<StatComponent>(params.entity1);

	//Get the components of the attackee (stats for taking damage and transform for knockback)
	StatComponent* stat2 = registry.GetComponent<StatComponent>(params.entity2);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);

	//Get the hitbox of the attacker and check if it's circular or convex, return out of here if the hitbox doesn't have the "canDealDamage" flag set
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
	//Do the same as the above but for the attackee, but returning out of here if the hitbox doesn't have the "canTakeDamage" flag set
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);
	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		//Check if possible to deal damage
		if (!hitbox2->circularFlags[params.hitboxID2].hitStaticHazard)
		{
			return;
		}
	}
	else //For convex hitboxes
	{
		if (!hitbox2->convexFlags[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].hitStaticHazard)
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

	/*
	Get hit, 3-step process
	1. Start: Lose health, then disable being able to take damage
	2. Continuous: Flash color using hue-shift, knockback depending on where we got attacked from
	3. End: Enable being able to take damage again, and maybe for safety reasons make sure our hue-shift is back to normal
	*/
	CollisionParamsComponent* eventParams = registry.AddComponent<CollisionParamsComponent>(params.entity2, params);
	//AddTimedEventComponentStart(params.entity2, params.entity2, 0.0f, nullptr);
	if (params.entity2.index == stateManager.player.index)
	{
		//Screen shaking
		int cameraShake = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, nullptr, ShakeCamera, CAMERA_CONSTANT_SHAKE_TIME, ResetCameraOffset, 0, 2);
	}
	int index = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, HazardBeginHit, MiddleHit, 0.5f, HazardEndHit); //No special condition for now
	//stat2->UpdateHealth(-stat1->damage);

	//Redraw UI (player healthbar) since someone will have taken damage at this point. 
	//If RedrawUI() is bad to call it's probably good to try and make sure this only happens if player is the one who got attacked
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

void AttackCollision(OnCollisionParameters& params)
{
	//Get the components of the attacker (only stats for dealing damage)
 	StatComponent* stat1 = registry.GetComponent<StatComponent>(params.entity1);

	//Get the components of the attackee (stats for taking damage and transform for knockback)
	StatComponent* stat2 = registry.GetComponent<StatComponent>(params.entity2);


	//Get the hitbox of the attacker and check if it's circular or convex, return out of here if the hitbox doesn't have the "canDealDamage" flag set
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);

	int counter = 0;
	int counter2 = 0;
	if (params.hitboxID1 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		//Check if possible to deal damage
		if (hitbox1->circularFlags[params.hitboxID1].canDealDamage)
		{
			counter++;
		}
	}
	else //For convex hitboxes
	{
		if (hitbox1->convexFlags[params.hitboxID1 - SAME_TYPE_HITBOX_LIMIT].canDealDamage)
		{
			counter++;
		}
	}

	//Do the same as the above but for the attackee, but returning out of here if the hitbox doesn't have the "canTakeDamage" flag set
	HitboxComponent* hitbox2 = registry.GetComponent<HitboxComponent>(params.entity2);
	if (params.hitboxID2 < SAME_TYPE_HITBOX_LIMIT)//For circular
	{
		//Check if possible to deal damage
		if (hitbox2->circularFlags[params.hitboxID2].canTakeDamage)
		{
			counter2++;
		}
	}
	else //For convex hitboxes
	{
		if (hitbox2->convexFlags[params.hitboxID2 - SAME_TYPE_HITBOX_LIMIT].canTakeDamage)
		{
			counter2++;
		}
	}

	//if nothing is hit, get out!
	if (counter == 0 || counter2 == 0)
	{
		return;
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

	/*
	Get hit, 3-step process
	1. Start: Lose health, then disable being able to take damage
	2. Continuous: Flash color using hue-shift, knockback depending on where we got attacked from
	3. End: Enable being able to take damage again, and maybe for safety reasons make sure our hue-shift is back to normal
	*/
	CollisionParamsComponent* eventParams = registry.AddComponent<CollisionParamsComponent>(params.entity2, params);
	//AddTimedEventComponentStart(params.entity2, params.entity2, 0.0f, nullptr);
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(params.entity2);
	if (enemy != nullptr)
	{
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(params.entity2);
		switch (enemy->type)
		{
		case EnemyType::hellhound:
			if (registry.GetComponent<StatComponent>(params.entity2)->GetHealth() > 0)
			{
				sfx->Play(Hellhound_Hurt, Channel_Base);
			}
			break;
		case EnemyType::eye:
			if (registry.GetComponent<StatComponent>(params.entity2)->GetHealth() > 0)
			{
				sfx->Play(Eye_Hurt, Channel_Base);
			}
			break;
		case EnemyType::skeleton:
			if (registry.GetComponent<StatComponent>(params.entity2)->GetHealth() > 0)
			{
				sfx->Play(Skeleton_Hurt, Channel_Base);
			}
			break;
		}
	}
	//Camera Shake
	int cameraShake = AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, nullptr, ShakeCamera, CAMERA_CONSTANT_SHAKE_TIME, ResetCameraOffset, 0, 2);
	//Hitstop, pause both animations for extra effect
	int index1 = AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, PauseAnimation, nullptr, FREEZE_TIME, ContinueAnimation, 0);
	int index2 = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, PauseAnimation, HitStop, FREEZE_TIME, ContinueAnimation, 0);
	//Freeze both entities as they hit eachother for extra effect
	int indexSpeedControl1 = AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, SetSpeedZero, nullptr, FREEZE_TIME, ResetSpeed, 0);
	int indexSpeedControl2 = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, SetSpeedZero, nullptr, FREEZE_TIME, ResetSpeed, 0);
	//Squash both entities for extra effect
	float squashKnockbackFactor = 1.0f + stat1->knockback * 0.1f;
	AddSquashStretch(params.entity2, Constant, 1.15f * squashKnockbackFactor, 1.1f, 0.75f);
	int squashStretch2 = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, ResetSquashStretch, SquashStretch, FREEZE_TIME, ResetSquashStretch, 0, 1);
	AddSquashStretch(params.entity1, Constant, 1.1f, 1.1f, 0.9f);
	int squashStretch1 = AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, ResetSquashStretch, SquashStretch, FREEZE_TIME, ResetSquashStretch, 0, 1);
	//Knockback mechanic
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(params.entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);
	AddKnockBack(params.entity1, SELF_KNOCKBACK_FACTOR * stat1->knockback * params.normal1X / transform1->mass, stat2->knockback * params.normal1Z / transform1->mass);
	AddKnockBack(params.entity2, stat1->knockback * params.normal2X / transform1->mass, stat1->knockback * params.normal2Z / transform1->mass);
	//Take damage and blinking
	int index3 = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, nullptr, BlinkColor, FREEZE_TIME + 0.2f, ResetColor); //No special condition for now
	int index4 = AddTimedEventComponentStartContinuousEnd(params.entity2, FREEZE_TIME, BeginHit, MiddleHit, FREEZE_TIME + 0.2f, EndHit); //No special condition for now
	//stat2->UpdateHealth(-stat1->damage);
	
	//Redraw UI (player healthbar) since someone will have taken damage at this point. 
	//If RedrawUI() is bad to call it's probably good to try and make sure this only happens if player is the one who got attacked
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
	//next level is shop so we set the paramaters in statemanager as so
	SetInPlay(false);
	SetInShop(true);
	
	LoadLevel(++stateManager.activeLevel);
}

