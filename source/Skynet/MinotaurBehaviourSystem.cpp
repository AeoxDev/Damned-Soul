#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Skynet\BehaviourHelper.h"
#include "EventFunctions.h"

#include <random>

void IdleBehaviour(EntityID& enemy, MinotaurBehaviour* mc, TransformComponent* mtc, StatComponent* stats, AnimationComponent* animComp)
{
	mc->timeCounter += GetDeltaTime();

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
	animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);

	if (mc->timeCounter >= mc->updateInterval)
	{
		mc->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		mc->goalDirectionX = randomX;
		mc->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		mc->updateInterval = randomInterval(gen);
	}

	SmoothRotation(mtc, mc->goalDirectionX, mc->goalDirectionZ);

	mtc->positionX += mtc->facingX * stats->GetSpeed() * 0.5f * GetDeltaTime();
	mtc->positionZ += mtc->facingZ * stats->GetSpeed() * 0.5f * GetDeltaTime();
	//TransformAccelerate(enemy, mtc->facingX * 0.5f, mtc->facingZ * 0.5f);
}

void ChargeBehaviour(EntityID& enemy, TransformComponent* ptc, MinotaurBehaviour* mc, TransformComponent* mtc, StatComponent* enemyStats, EnemyComponent* enemyComp, AnimationComponent* enemyAnim, float goalDirectionX = 0.0f, float goalDirectionZ = 0.0f, bool path = false)
{
	if (!mc->charging)
	{
		enemyAnim->aAnimTime = 0;

		if (enemyAnim->aAnim != ANIMATION_WALK)
		{
			enemyAnim->aAnim = ANIMATION_WALK;
			enemyAnim->aAnimIdx = 0;
			enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor * (1 + mc->charging);
			ANIM_BRANCHLESS(enemyAnim);
		}

		//reset values
		mc->attackTimer = 0;
		mc->charging = true;

		//while charging disable hitboxes
		//SetHitboxIsMoveable(enemy, 0, false);
		//SetHitboxIsMoveable(enemy, 1, false);
		enemyStats->SetKnockbackMultiplier(2.0f);

		//direction from the enemy towards the player
		float dirX = ptc->positionX - mtc->positionX;
		float dirZ = ptc->positionZ - mtc->positionZ;

		Normalize(dirX, dirZ);

		//target is the stopping point of the charge, a set distance behind the players position
		mc->targetX = ptc->positionX + dirX * 3.0f;
		mc->targetZ = ptc->positionZ + dirZ * 3.0f;

		mc->chargeDirX = dirX; // charge direction is only set once per charge and will not change
		mc->chargeDirZ = dirZ;

		SmoothRotation(mtc, mc->chargeDirX, mc->chargeDirZ, 40.0f);

		AddTimedEventComponentStartContinuousEnd(enemy, 0.0f, nullptr, ChargeColorFlash, mc->aimDuration - 0.2f, ResetColor);
	}

	if (mc->aimTimer < mc->aimDuration)
	{
		mc->goalDirectionX = ptc->positionX - mtc->positionX;
		mc->goalDirectionZ = ptc->positionZ - mtc->positionZ;

		Normalize(mc->goalDirectionX, mc->goalDirectionZ);

		SmoothRotation(mtc, mc->goalDirectionX, mc->goalDirectionZ, 30.f);
		mc->aimTimer += GetDeltaTime();
	}
	else
	{
		if (enemyAnim->aAnim != ANIMATION_ATTACK)
		{
			enemyAnim->aAnim = ANIMATION_ATTACK;
			enemyAnim->aAnimIdx = 0;
			enemyAnim->aAnimTime = 0.0f;
		}
		//slightly adjust the charging direction based on player position
		float playerOffsetX = ptc->positionX - ptc->lastPositionX;
		float playerOffsetZ = ptc->positionZ - ptc->lastPositionZ;
		Normalize(playerOffsetX, playerOffsetZ);
		
		float offsetScalar = 0.2f;

		mc->chargeDirX += playerOffsetX * offsetScalar;
		mc->chargeDirZ += playerOffsetZ * offsetScalar;
		Normalize(mc->chargeDirX, mc->chargeDirZ);

		//calculate the current direction towards the target position
		float dirX = (mc->targetX - mtc->positionX); //no workie
		float dirZ = (mc->targetZ - mtc->positionZ); //no workie
		Normalize(dirX, dirZ);

		//scalar between the current direction and the original chagre direction
		float scalar = dirX * mc->chargeDirX + dirZ * mc->chargeDirZ;

		//If charging scalar point direction > 0.0, charge
		if (scalar > 0 && mc->chargeTimer < 3.0f)
		{
			mc->chargeTimer += GetDeltaTime();
 			SmoothRotation(mtc, mc->chargeDirX, mc->chargeDirZ, 30.0f);

			mtc->positionX += mc->chargeDirX * enemyStats->GetSpeed() * 6.f * GetDeltaTime();
			mtc->positionZ += mc->chargeDirZ * enemyStats->GetSpeed() * 6.f * GetDeltaTime();

			SetHitboxActive(enemy, enemyComp->attackHitBoxID, true);
			SetHitboxCanDealDamage(enemy, enemyComp->attackHitBoxID, true);
		}
		else //else charge is finished
		{
			enemyStats->SetKnockbackMultiplier(1.0f);
			//reenable hitboxes
			//SetHitboxIsMoveable(enemy, 0, true);
			//SetHitboxIsMoveable(enemy, 1, true);
			SetHitboxActive(enemy, enemyComp->attackHitBoxID, false);
			SetHitboxCanDealDamage(enemy, enemyComp->attackHitBoxID, false);

			//reset values
			mc->charging = false;
			mc->chargeAttackSoundPlaying = false;
			//mc->attackTimer = 0;
			mc->attackStunTimer = 0;

			enemyAnim->aAnimTime = 0.0f;
			enemyAnim->aAnim = ANIMATION_IDLE;
			enemyAnim->aAnimIdx = 1;
		}
	}
}

bool MinotaurBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	MinotaurBehaviour* minoComp = nullptr;
	TransformComponent* minoTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enmComp = nullptr;
	AnimationComponent* enemyAnim = nullptr;

	bool updateGridOnce = true;

	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));


	for (auto enemyEntity : View<MinotaurBehaviour, TransformComponent, StatComponent, EnemyComponent, AnimationComponent>(registry))
	{
		minoComp = registry.GetComponent<MinotaurBehaviour>(enemyEntity);
		minoTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

		//Find a player to kill.
		if (enmComp != nullptr)
		{
			if (enmComp->lastPlayer.index == -1)
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
					enmComp->lastPlayer = playerEntity;
					if (rand() % 2)
					{
						break;
					}
				}
			}
			else
			{
				playerComponent = registry.GetComponent<PlayerComponent>(enmComp->lastPlayer);
				playerTransformCompenent = registry.GetComponent<TransformComponent>(enmComp->lastPlayer);
				playerStats = registry.GetComponent< StatComponent>(enmComp->lastPlayer);
				if (playerComponent == nullptr)
				{
					for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
					{
						enmComp->lastPlayer.index = -1;
					}
				}
			}
		}

		//ACTUAL BEHAVIOUR
		if (minoTransformComponent != nullptr && playerTransformCompenent != nullptr && enemyAnim != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(minoTransformComponent->positionX, minoTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);

			if (minoComp->attackStunTimer <= minoComp->attackStunDuration)
			{
				minoComp->attackStunTimer += GetDeltaTime();
			}
			else if (minoComp->charging || distance <= 40.0f)
			{
				if (!minoComp->chargeAttackSoundPlaying)
				{
					minoComp->chargeAttackSoundPlaying = true;
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(enemyEntity);
					sfx->Play(Eye_Attack, Channel_Base); //minotaur charge sound???
					minoComp->chargeTimer = 0.0f;
				}
				ChargeBehaviour(enemyEntity, playerTransformCompenent, minoComp, minoTransformComponent, enemyStats, enmComp, enemyAnim);
			}
			else if (minoComp->jumping) // jump
			{
				minoComp->airTimer += GetDeltaTime();
				if (minoComp->airTimer <= minoComp->airDuration) // fly up in the air
				{
					minoTransformComponent->positionY += enemyStats->GetSpeed() * 7.f * GetDeltaTime();
				}
				else  // fly down from air
				{
					if (playerComponent != nullptr && updateGridOnce && minoComp->hasLandingPos == false)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesLuciferJump(valueGrid); //generate a gridmap
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true; //illegal grid
							continue;
						}
						TransformComponent landingPosition = FindRetreatTile(valueGrid, playerTransformCompenent, 10.f, 20.f); // where to land
						minoComp->hasLandingPos = true;
						minoTransformComponent->positionX = landingPosition.positionX; //teleport in the air basically
						minoTransformComponent->positionZ = landingPosition.positionZ; // happens once
					}
					if (minoTransformComponent->positionY > 0.f) // still in the air
					{
						minoTransformComponent->positionY -= enemyStats->GetSpeed() * 12.f * GetDeltaTime();
					}
					else // on ground or below ground, set to ground and SHOCKWAVE with timed event
					{
						//reset variables to beheaviour. No more jump, get dazed
						minoComp->jumping = false;
						//minoComp->isDazed = true;
						SetHitboxActive(enemyEntity, 0, true);
						SetHitboxActive(enemyEntity, 1, true);
						SetHitboxActive(enemyEntity, 2, true);
						minoTransformComponent->positionY = 0.f;

						//shockwave here
						AddTimedEventComponentStartContinuousEnd(enemyEntity, 0.0f, BossShockwaveStart, BossShockwaveExpand, 4.0f, BossShockwaveEnd, 0, 1);
					}
				}
			}
			else // Nothing to do, go idle 
			{ 
				IdleBehaviour(enemyEntity, minoComp, minoTransformComponent, enemyStats, enemyAnim);
			}
		}
	}
	
	free(valueGrid);
	return true;
}