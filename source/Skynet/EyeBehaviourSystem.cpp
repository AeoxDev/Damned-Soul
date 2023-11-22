#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include "UI/UIRenderer.h"
#include "EventFunctions.h"
#include <random>

void RetreatBehaviour(PlayerComponent* pc, TransformComponent* ptc, EyeBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats, AnimationComponent* enemyAnim, PathfindingMap* valueGrid)
{
	// Regular walk
	if (enemyAnim->aAnim != ANIMATION_WALK)
	{
		enemyAnim->aAnim = ANIMATION_WALK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}
	
	//using charge timer as a safetynet 
	ec->chargeTimer += GetDeltaTime();
	if (!ec->retreating)
	{
		ec->retreating = true;
		float minRange = 20.0f;
		float maxRange = 30.0f;

		TransformComponent newTile = FindRetreatTile(valueGrid, etc, 20.f, 30.f);

		float dirToPlayerX = ptc->positionX - etc->positionX;
		float dirToPlayerZ = ptc->positionZ - etc->positionZ;
		Normalize(dirToPlayerX, dirToPlayerZ);

		float dirToNewTileX = newTile.positionX - etc->positionX;
		float dirToNewTileZ = newTile.positionZ - etc->positionZ;
		Normalize(dirToNewTileX, dirToNewTileZ);

		float dotX = dirToNewTileX * dirToPlayerX;
		float dotZ = dirToNewTileZ * dirToPlayerZ;
		int iterations = 0;
		while ((dotX > 0.0f || dotZ > 0.0f) && iterations > 1000)
		{
			newTile = FindRetreatTile(valueGrid, etc, 20.f, 30.f);

			float dirToNewTileX = newTile.positionX - etc->positionX;
			float dirToNewTileZ = newTile.positionZ - etc->positionZ;
			Normalize(dirToNewTileX, dirToNewTileZ);

			dotX = dirToNewTileX * dirToPlayerX;
			dotZ = dirToNewTileZ * dirToPlayerZ;
			iterations++;
		}

		ec->targetX = newTile.positionX;
		ec->targetZ = newTile.positionZ;

		ec->goalDirectionX = dirToNewTileX;
		ec->goalDirectionZ = dirToNewTileZ;

		SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);

		etc->positionX += dirToNewTileX * enemyStats->GetSpeed() * GetDeltaTime();
		etc->positionZ += dirToNewTileZ * enemyStats->GetSpeed() * GetDeltaTime();
	}
	else if(ec->chargeTimer < 1.5f)
	{
		auto dist = Calculate2dDistance(etc->positionX, etc->positionZ, ec->targetX, ec->targetZ);
		if (dist > 1.0f)
		{
			SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
			etc->positionX += ec->goalDirectionX * enemyStats->GetSpeed() * GetDeltaTime();
			etc->positionZ += ec->goalDirectionZ * enemyStats->GetSpeed() * GetDeltaTime();
		}
		else
		{
			ec->retreating = false;
			ec->chargeTimer = 0.0f;
		}
	}
	else 
	{
		ec->retreating = false;
		ec->chargeTimer = 0.0f;
	}
}

bool CombatBehaviour(EntityID entity, PlayerComponent*& pc, TransformComponent*& ptc, EyeBehaviour*& ec, TransformComponent*& etc, StatComponent*& enemyStats, StatComponent*& playerStats, AnimationComponent* enemyAnim)
{
	//if you just attacked go back to circle behaviour
	if (ec->attackTimer < enemyStats->GetAttackSpeed())
	{
		ec->attackTimer += GetDeltaTime();
		return false;
	}
	//rotate eye in order to shoot at the player
	else if (ec->aimTimer < ec->aimDuration)
	{
		if (enemyAnim->aAnim != ANIMATION_ATTACK)
		{
			enemyAnim->aAnim = ANIMATION_ATTACK;
			enemyAnim->aAnimIdx = 1;
			enemyAnim->aAnimTime = 0.0f;
		}
		
		ec->goalDirectionX = ptc->positionX - etc->positionX;
		ec->goalDirectionZ = ptc->positionZ - etc->positionZ;

		Normalize(ec->goalDirectionX, ec->goalDirectionZ);

		SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
		ec->aimTimer += GetDeltaTime();
		ec->attackTimer += GetDeltaTime();
		return true;
	}
	else // yes, we can indeed attack. 
	{
		ec->attackTimer = 0;
		ec->aimTimer = 0;
		ec->specialCounter++; //increase the special counter for special attack
		ec->attackStunTimer = 0;
		
		enemyAnim->aAnim = ANIMATION_IDLE;
		enemyAnim->aAnimIdx = 1;
		enemyAnim->aAnimTime = 0.0f;

		//set direction for attack
		float dx = ptc->positionX - etc->positionX;
		float dz = ptc->positionZ - etc->positionZ;
		
		Normalize(dx, dz);

		CreateProjectile(entity, dx, dz, 1);
		return true;
	}
}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, EyeBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats, StatComponent* playerStats, AnimationComponent* enemyAnim)
{
	if (enemyAnim->aAnim != ANIMATION_WALK && enemyAnim->aAnimIdx != 0)
	{
		enemyAnim->aAnim = ANIMATION_WALK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

	float magnitude = 0.f;
	float dirX = 0.f;
	float dirZ = 0.f;

	if (ec->clockwiseCircle) //clockwise
	{
		dirX = -ec->goalDirectionZ;
		dirZ = ec->goalDirectionX;
	}
	else // counter clockwise
	{
		dirX = ec->goalDirectionZ;
		dirZ = -ec->goalDirectionX;
	}

	Normalize(dirX, dirZ);

	etc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
	etc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
	ec->goalDirectionX = ptc->positionX - etc->positionX;
	ec->goalDirectionZ = ptc->positionZ - etc->positionZ;

	SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
}

void IdleBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats, AnimationComponent* enemyAnim)
{
	if (enemyAnim->aAnim != ANIMATION_IDLE)
	{
		enemyAnim->aAnim = ANIMATION_IDLE;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

	if (eyeComponent->timeCounter >= eyeComponent->updateInterval)
	{
		eyeComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		eyeComponent->goalDirectionX = randomX;
		eyeComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		eyeComponent->updateInterval = randomInterval(gen);
	}

	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);
	eyeTransformComponent->positionX += eyeTransformComponent->facingX * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
	eyeTransformComponent->positionZ += eyeTransformComponent->facingZ * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
	eyeComponent->timeCounter += GetDeltaTime();
}

void ChargeBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats, StatComponent* playerStats, HitboxComponent* enemyHitbox, EntityID eID, EnemyComponent* enemComp, AnimationComponent* enemyAnim)
{
	if (!eyeComponent->charging)
	{
		enemyAnim->aAnimTime = 0;

		//reset value and calculate new breakpoint
		eyeComponent->attackTimer = 0;
		eyeComponent->specialCounter = 0;
		eyeComponent->charging = true;

		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [2.0, 5.0]
		std::uniform_real_distribution<float> distribution(2.0f, 5.0f);
		eyeComponent->specialBreakpoint = (int)distribution(gen);


		//while charging disable hitboxes
		SetHitboxIsMoveable(eID, 0, false);
		SetHitboxIsMoveable(eID, 1, false);
		enemyStats->SetKnockbackMultiplier(2.0f);
		//direction from the enemy towards the player
		float dirX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
		float dirZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

		Normalize(dirX, dirZ);

		//target is the stopping point of the charge, a set distance behind the players position
		eyeComponent->targetX = playerTransformCompenent->positionX + dirX * 10.0f;
		eyeComponent->targetZ = playerTransformCompenent->positionZ + dirZ * 10.0f;

		eyeComponent->changeDirX = dirX; // charge direction is only set once per charge and will not change
		eyeComponent->changeDirZ = dirZ; 

		//change what direction the eye is circling after each dash
		(eyeComponent->clockwiseCircle == true) ? eyeComponent->clockwiseCircle = false : eyeComponent->clockwiseCircle = true;

		SmoothRotation(eyeTransformComponent, eyeComponent->changeDirX, eyeComponent->changeDirZ, 40.0f);
		AddTimedEventComponentStartContinuousEnd(eID, 0.0f, nullptr, ChargeColorFlash, eyeComponent->aimDuration - 0.2f, ResetColor);
	}
	else if (eyeComponent->aimTimer < eyeComponent->aimDuration)
	{
		eyeComponent->goalDirectionX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
		eyeComponent->goalDirectionZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

		Normalize(eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);

		SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ, 30.f);
		eyeComponent->aimTimer += GetDeltaTime();
		eyeComponent->attackTimer += GetDeltaTime();
	}
	else 
	{
		if (enemyAnim->aAnim != ANIMATION_ATTACK)
		{
			enemyAnim->aAnim = ANIMATION_ATTACK;
			enemyAnim->aAnimIdx = 0;
			enemyAnim->aAnimTime = 0.0f;
		}
		
		//calculate the current direction towards player
		float dirX = (eyeComponent->targetX - eyeTransformComponent->positionX);
		float dirZ = (eyeComponent->targetZ - eyeTransformComponent->positionZ);

		Normalize(dirX, dirZ);

		//scalar between the current direction and the original chagre direction
		float scalar = dirX * eyeComponent->changeDirX + dirZ * eyeComponent->changeDirZ;

		//If charging scalar point direction > 0.0, charge
		if (scalar > 0 && eyeComponent->chargeTimer < 3.0f)
		{
			eyeComponent->chargeTimer += GetDeltaTime();
			SmoothRotation(eyeTransformComponent, eyeComponent->changeDirX, eyeComponent->changeDirZ, 30.0f);

			eyeTransformComponent->positionX += eyeComponent->changeDirX * enemyStats->GetSpeed() * 6.f * GetDeltaTime();
			eyeTransformComponent->positionZ += eyeComponent->changeDirZ * enemyStats->GetSpeed() * 6.f * GetDeltaTime();

			SetHitboxActive(eID, enemComp->attackHitBoxID, true);
			SetHitboxCanDealDamage(eID, enemComp->attackHitBoxID, true);
		}
		else //else charge is finished
		{
			enemyStats->SetKnockbackMultiplier(1.0f);
			//reenable hitboxes
			SetHitboxIsMoveable(eID, 0, true);
			SetHitboxIsMoveable(eID, 1, true);
			SetHitboxActive(eID, enemComp->attackHitBoxID, false);
			SetHitboxCanDealDamage(eID, enemComp->attackHitBoxID, false);

			//reset values
			eyeComponent->charging = false;
			eyeComponent->chargeAttackSoundPlaying = false;
			eyeComponent->attackTimer = 0;
			eyeComponent->attackStunTimer = 0;
			eyeComponent->dealtDamage = false;
			
			enemyAnim->aAnimTime = 0.0f;
			enemyAnim->aAnim = ANIMATION_IDLE;
			enemyAnim->aAnimIdx = 1;
		}
	}
}

bool EyeBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	EyeBehaviour* eyeComponent = nullptr;
	TransformComponent* eyeTransformComponent = nullptr;
	HitboxComponent* enemyHitbox = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enemComp = nullptr;
	DebuffComponent* debuff = nullptr;

	bool hasUpdatedMap = false;
	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));
	
	//Find available entity
	for (auto enemyEntity : View<EyeBehaviour, TransformComponent, HitboxComponent, EnemyComponent>(registry))
	{
		SetLightColor(enemyEntity, 0.3f, 0.3f, 0.3f);
		eyeComponent = registry.GetComponent<EyeBehaviour>(enemyEntity);
		eyeTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		enemyHitbox = registry.GetComponent<HitboxComponent>(enemyEntity);
		enemComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		AnimationComponent* enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

		debuff = registry.GetComponent<DebuffComponent>(enemyEntity);
		if (debuff && debuff->m_frozen)
		{
			continue; // frozen, won't do behavior stuff
		}


		//Find a player to kill.
		if (enemComp->lastPlayer.index == -1)
		{
			for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
			{
				if (enemyEntity.index == playerEntity.index)
				{
					continue;
				}

				playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
				playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
				playerStats = registry.GetComponent< StatComponent>(playerEntity);
				enemComp->lastPlayer = playerEntity;
				if (rand() % 2)
				{
					break;
				}
			}
		}
		else
		{
			playerComponent = registry.GetComponent<PlayerComponent>(enemComp->lastPlayer);
			playerTransformCompenent = registry.GetComponent<TransformComponent>(enemComp->lastPlayer);
			playerStats = registry.GetComponent< StatComponent>(enemComp->lastPlayer);
			if (playerComponent == nullptr)
			{
				enemComp->lastPlayer.index = -1;
			}
		}
		

		if (enemyStats->GetHealth() > 0 && eyeComponent != nullptr && playerTransformCompenent != nullptr && enemyHitbox != nullptr && enemComp != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(eyeTransformComponent->positionX, eyeTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			if (eyeComponent->attackStunTimer <= eyeComponent->attackStunDuration)
			{
				//do nothing
			}
			else if ((distance < 15.0f || eyeComponent->retreating) && !eyeComponent->charging) // Retreat to safe distance if not charging
			{
				if (hasUpdatedMap == false)
				{
					hasUpdatedMap = true;
					CalculateGlobalMapValuesImp(valueGrid);
				}
				RetreatBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyAnim, valueGrid);
			}
			else if (eyeComponent->charging || (eyeComponent->specialCounter >= eyeComponent->specialBreakpoint && eyeComponent->attackTimer >= enemyStats->GetAttackSpeed())) //if special is ready or is currently doing special
			{
				//CHAAAAARGE
				if (!eyeComponent->chargeAttackSoundPlaying)
				{
					eyeComponent->chargeAttackSoundPlaying = true;
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(enemyEntity);
					sfx->Play(Eye_Attack, Channel_Base);
					eyeComponent->chargeTimer = 0.0f;
				}
				ChargeBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyHitbox, enemyEntity, enemComp, enemyAnim);

			}
			else if (distance <= 45.0f + eyeComponent->circleBehaviour) // circle player & attack when possible (WIP)
			{
				if (!CombatBehaviour(enemyEntity, playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyAnim))
					CircleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyAnim);
			}
			else // idle
			{
				enemComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyAnim);
			}
		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enemComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyAnim);
		}

		eyeComponent->attackStunTimer += GetDeltaTime();
		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
		ANIM_BRANCHLESS(enemyAnim);
		TransformDecelerate(enemyEntity);
	}

	free(valueGrid);

	return true;
}