#include "CollisionFunctions.h"
#include "Components.h"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "Level.h"
#include "UIRenderer.h"
#include <assert.h>
#include "UIRenderer.h"
#include "Relics/RelicFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "EventFunctions.h"
#include "Levels/LevelHelper.h"
#include <cmath>

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
void HellhoundBreathAttackCollision(OnCollisionParameters& params)
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

	CollisionParamsComponent* eventParams = registry.AddComponent<CollisionParamsComponent>(params.entity2, params);
	
	if (params.entity2.index == stateManager.player.index)
	{
		//Screen shaking
		int cameraShake = AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, nullptr, ShakeCamera, CAMERA_CONSTANT_SHAKE_TIME, ResetCameraOffset, 0, 2);
	}

	AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, HazardBeginHit, MiddleHit, 0.5f, HazardEndHit); //No special condition for now

	//If the entity that got attacked was the player, RedrawUI since we need to update player healthbar
	if (registry.GetComponent<PlayerComponent>(params.entity2) != nullptr)
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

void HazardAttackCollision(OnCollisionParameters& params)
{
	StatComponent* stat = registry.GetComponent<StatComponent>(params.entity2);
	StaticHazardComponent* hazard = registry.GetComponent<StaticHazardComponent>(params.entity1);

	TransformComponent* p = registry.GetComponent<TransformComponent>(params.entity2);
	HitboxComponent* h = registry.GetComponent<HitboxComponent>(params.entity2);
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(params.entity2);
	if (HitboxCanHitGI(params.entity2))
	{
		int r = hazard->type;//PixelValueOnPosition(geoCo, p);
		int takeDamage = 0;
		ProjectileComponent* proj = nullptr;

		switch (r)
		{
		case 0:
			p->positionX = 0.f;
			p->positionZ = 0.f;
			break;
		case 1:
			//Footstep sound here?
			stat->m_acceleration = stat->m_baseAcceleration;
			break;
		case HAZARD_LAVA:
			if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
			{
				anim->aAnimTimeFactor = stat->lavaAnimFactor;
				AddTimedEventComponentStart(params.entity2, 0.01f, ContinueAnimation, 0, 2);
			}
			stat->m_acceleration = stat->m_baseAcceleration * stat->lavaAccelFactor;

			HazardDamageHelper(params.entity2, 25.f);
			//takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
			break;
		case HAZARD_CRACK:
			if (!stat->canWalkOnCrack)
			{
				//Detect edge
				//Edge direction
				p->positionX -= p->facingX * GetDeltaTime() * stat->GetSpeed();
				p->positionZ -= p->facingZ * GetDeltaTime() * stat->GetSpeed();
			}
			break;
		case HAZARD_ACID://Lava but more damage
			if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
			{
				anim->aAnimTimeFactor = stat->acidAnimFactor;
				AddTimedEventComponentStart(params.entity2, 0.01f, ContinueAnimation, 0, 2);
			}
			stat->m_acceleration = stat->m_baseAcceleration * stat->acidAccelFactor;

			HazardDamageHelper(params.entity2, 20.f);
			break;
		case HAZARD_ICE:
			//ICE:
			if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
			{
				anim->aAnimTimeFactor = stat->iceAnimFactor;
				AddTimedEventComponentStart(params.entity2, 0.01f, ContinueAnimation, 0, 2);
			}
			stat->m_acceleration = stat->m_baseAcceleration * stat->iceAccelFactor;

			//HazardDamageHelper(entity, 25.f);
			//takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
			break;
		default:
			break;
		}
	}
}

bool IsDamageCollisionValid(OnCollisionParameters& params)
{
	//Get the components of the attacker (only stats for dealing damage)
	StatComponent* stat1 = registry.GetComponent<StatComponent>(params.entity1);

	//Get the components of the attackee (stats for taking damage and transform for knockback)
	StatComponent* stat2 = registry.GetComponent<StatComponent>(params.entity2);

	//Get the hitbox of the attacker and check if it's circular or convex, return out of here if the hitbox doesn't have the "canDealDamage" flag set
	//Realistically we can skip this part since the player will be the only entity with a dash hitbox but buhhh
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
		return false;
	}

	//Check if hitbox already dealt damage
	for (size_t i = 0; i < HIT_TRACKER_LIMIT; i++)
	{
		//If already in hit tracker: no proc
		if (hitbox1->hitTracker[i].active && hitbox1->hitTracker[i].entity.index == params.entity2.index)
		{
			return false;
		}
	}

	//We made it
	return true;
}

void ApplyHitFeedbackEffects(OnCollisionParameters& params)
{
	//Get relevant components
	StatComponent* stat1 = registry.GetComponent<StatComponent>(params.entity1);
	StatComponent* stat2 = registry.GetComponent<StatComponent>(params.entity2);
	CollisionParamsComponent* eventParams = registry.AddComponent<CollisionParamsComponent>(params.entity2, params);

	//Run timed events for on-hit:
	//Camera shake
	AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, nullptr, ShakeCamera, CAMERA_CONSTANT_SHAKE_TIME, ResetCameraOffset, 0, 2);

	//Hitstop, pause both animations for extra effect
	AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, PauseAnimation, nullptr, FREEZE_TIME, ContinueAnimation, 0);
	AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, PauseAnimation, HitStop, FREEZE_TIME, ContinueAnimation, 0);

	//Freeze both entities as they hit eachother for extra effect
	AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, SetSpeedZero, nullptr, FREEZE_TIME, ResetSpeed, 0);
	AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, SetSpeedZero, nullptr, FREEZE_TIME, ResetSpeed, 0);

	//Squash both entities for extra effect
	float squashKnockbackFactor = 1.0f + stat1->GetKnockback() * 0.1f;
	AddSquashStretch(params.entity2, Constant, 1.15f * squashKnockbackFactor, 1.1f, 0.75f);
	AddTimedEventComponentStartContinuousEnd(params.entity2, 0.0f, ResetSquashStretch, SquashStretch, FREEZE_TIME, ResetSquashStretch, 0, 1);
	AddSquashStretch(params.entity1, Constant, 1.1f, 1.1f, 0.9f);
	AddTimedEventComponentStartContinuousEnd(params.entity1, 0.0f, ResetSquashStretch, SquashStretch, FREEZE_TIME, ResetSquashStretch, 0, 1);

	//Knockback
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(params.entity1);
	float frictionKnockbackFactor1 = stat1->m_acceleration / stat1->m_baseAcceleration;
	float frictionKnockbackFactor2 = stat2->m_acceleration / stat2->m_baseAcceleration;
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(params.entity2);
	AddKnockBack(params.entity1, SELF_KNOCKBACK_FACTOR * stat1->GetKnockback() * params.normal1X / (transform1->mass * frictionKnockbackFactor1) , stat2->GetKnockback() * params.normal1Z / (transform1->mass * frictionKnockbackFactor1));
	AddKnockBack(params.entity2, stat1->GetKnockback() * params.normal2X / (transform2->mass * frictionKnockbackFactor2), frictionKnockbackFactor2 * stat1->GetKnockback() * params.normal2Z / (transform2->mass * frictionKnockbackFactor2));
}

void PlayHitSound(OnCollisionParameters& params)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(params.entity2);
	if (enemy)
	{
		enemy->lastPlayer = params.entity1;
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
	else
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(params.entity2);
		if (player != nullptr)
		{
			SoundComponent* sfx = registry.GetComponent<SoundComponent>(params.entity2);
			sfx->Play(Player_Hurt, Channel_Base);
		}
	}
}

void DashCollision(OnCollisionParameters& params)
{
	//Return out of the function if the damage collision isn't valid (attacker needs to be able to deal damage, defender needs to be able to take damage, etc)
	if (IsDamageCollisionValid(params) == false)
	{
		return;
	}

	//Apply hit-feedback like camera shake, hitstop and knockback
	ApplyHitFeedbackEffects(params);

	//Deal damage to the defender and make their model flash red
	AddTimedEventComponentStartContinuousEnd(params.entity2, FREEZE_TIME, DashBeginHit, MiddleHit, FREEZE_TIME + 0.2f, EndHit); //No special condition for now

	//Play entity hurt sounds
	PlayHitSound(params);

	//If the entity that got attacked was the player, RedrawUI since we need to update player healthbar
	if(registry.GetComponent<PlayerComponent>(params.entity2) != nullptr)
		RedrawUI();

	//Lastly set for hitboxTracker[]
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
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
	//Return out of the function if the damage collision isn't valid (attacker needs to be able to deal damage, defender needs to be able to take damage, etc)
	if (IsDamageCollisionValid(params) == false)
	{
		return;
	}

	//Apply hit-feedback like camera shake, hitstop and knockback
	ApplyHitFeedbackEffects(params);

	//Deal damage to the defender and make their model flash red
	auto charge = registry.GetComponent<ChargeAttackArgumentComponent>(params.entity1);
	if (charge)
		AddTimedEventComponentStartContinuousEnd(params.entity2, FREEZE_TIME, BeginHit, MiddleHit, FREEZE_TIME + 0.2f, EndHit, CONDITION_CHARGE);
	else
		AddTimedEventComponentStartContinuousEnd(params.entity2, FREEZE_TIME, BeginHit, MiddleHit, FREEZE_TIME + 0.2f, EndHit);

	//Play entity hurt sounds
	PlayHitSound(params);

	//If the entity that got attacked was the player, RedrawUI since we need to update player healthbar
	if (registry.GetComponent<PlayerComponent>(params.entity2) != nullptr)
		RedrawUI();

	// Only hit the first enemy
	//SetHitboxCanDealDamage(params.entity1, params.hitboxID1, false);

	//Lastly set for hitboxTracker[]
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
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

void ShockWaveAttackCollision(OnCollisionParameters& params)
{
	
	//Return out of the function if the damage collision isn't valid (attacker needs to be able to deal damage, defender needs to be able to take damage, etc)
	if (IsDamageCollisionValid(params) == false)
	{
		return;
	}

	//Check if within radius
	//Get distance from center of circle.
	float dist = sqrtf((params.pos1X - params.pos2X) * (params.pos1X - params.pos2X) + (params.pos1Z - params.pos2Z) * (params.pos1Z - params.pos2Z));
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(params.entity1);
	float sectorWidth = 2.0f;
	if (dist < hitbox->circleHitbox[params.hitboxID1].radius - sectorWidth)
	{
		return;
	}

	//Apply hit-feedback like camera shake, hitstop and knockback
	ApplyHitFeedbackEffects(params);

	//Deal damage to the defender and make their model flash red
	AddTimedEventComponentStartContinuousEnd(params.entity2, FREEZE_TIME, BeginHit, MiddleHit, FREEZE_TIME + 0.2f, EndHit); //No special condition for now

	//Play entity hurt sounds
	PlayHitSound(params);

	//If the entity that got attacked was the player, RedrawUI since we need to update player healthbar
	if (registry.GetComponent<PlayerComponent>(params.entity2) != nullptr)
		RedrawUI();

	//Lastly set for hitboxTracker[]
	HitboxComponent* hitbox1 = registry.GetComponent<HitboxComponent>(params.entity1);
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

void ProjectileAttackCollision(OnCollisionParameters& params)
{
	ProjectileComponent* projectile = registry.GetComponent<ProjectileComponent>(params.entity1);
	if (projectile != nullptr)
	{
		if (params.entity2.index == projectile->parentID.index)
		{
			return;
		}
	}

	//Apply hit-feedback like camera shake, hitstop and knockback
	ApplyHitFeedbackEffects(params);

	//Deal damage to the defender and make their model flash red
	AddTimedEventComponentStartContinuousEnd(params.entity2, FREEZE_TIME, BeginHit, MiddleHit, FREEZE_TIME + 0.2f, EndHit); //No special condition for now

	//Play entity hurt sounds
	PlayHitSound(params);

	//If the entity that got attacked was the player, RedrawUI since we need to update player healthbar
	if (registry.GetComponent<PlayerComponent>(params.entity2) != nullptr)
		RedrawUI();
	
	AddTimedEventComponentStartEnd(params.entity1, 0.0f, BeginDestroyProjectile, 1.0f, EndDestroyProjectile, 0, 2);
}

void LoadNextLevel(OnCollisionParameters& params)
{
	//next level is shop so we set the paramaters in statemanager as so
	if (params.entity2.index == stateManager.player.index)
	{
		SetInPlay(false);
		SetInShop(true);

		LoadLevel(++stateManager.activeLevel);
	}
	
}

