#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Skynet\BehaviourHelper.h"
#include "EventFunctions.h"

#include <random>
#include <math.h>

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
		mc->chargeDirX = ptc->positionX - mtc->positionX;
		mc->chargeDirZ = ptc->positionZ - mtc->positionZ;
		Normalize(mc->chargeDirX, mc->chargeDirZ);

		mc->goalDirectionX = mc->chargeDirX;
		mc->goalDirectionZ = mc->chargeDirZ;


		SmoothRotation(mtc, mc->goalDirectionZ, mc->goalDirectionZ, 40.0f);

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

		//direction from the enemy towards the player
		float dirToPlayerX = ptc->positionX - mtc->positionX;
		float dirToPlayerZ = ptc->positionZ - mtc->positionZ;
		Normalize(dirToPlayerX, dirToPlayerZ);

		float scalar = mc->chargeDirX * dirToPlayerX + mc->chargeDirZ * dirToPlayerZ;

		if (scalar > 0.0f && !mc->hasChargingDir)
		{
			mc->goalDirectionX = ptc->positionX - mtc->positionX;
			mc->goalDirectionZ = ptc->positionZ - mtc->positionZ;
			Normalize(mc->goalDirectionX, mc->goalDirectionZ);
		}
		else
		{
			mc->goalDirectionX = mtc->facingX;
			mc->goalDirectionZ = mtc->facingZ;
			mc->hasChargingDir = true;
		}

		//If charging scalar point direction > 0.0, charge
		if (mc->chargeTimer < mc->chargeDuration)
		{
			mc->chargeTimer += GetDeltaTime();
 			SmoothRotation(mtc, mc->goalDirectionX, mc->goalDirectionZ, 2.0f);

			mtc->positionX += mtc->facingX * enemyStats->GetSpeed() * 12.f * GetDeltaTime();
			mtc->positionZ += mtc->facingZ * enemyStats->GetSpeed() * 12.f * GetDeltaTime();
			//TransformAccelerate(enemy, mtc->facingX * 6.0f, mtc->facingZ * 6.0f);

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
			mc->hasChargingDir = false;
			mc->attackStunTimer = 0;
			mc->aimTimer = 0;
			//mc->attackTimer = 0;

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
			else if (minoComp->charging )
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
			else if (minoComp->jumping || distance <= 40.0f) // jump
			{
				if (minoComp->jumpTimer <= minoComp->JumpDuration)
				{
					AddTimedEventComponentStartContinuousEnd(enemyEntity, 0.0f, BossShockwaveStart, BossShockwaveExpand, 4.0f, BossShockwaveEnd, 0, 1);

				}

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
				
					minoComp->airTimer += GetDeltaTime();
				}
				minoComp->jumpTimer += GetDeltaTime();
			}
			else // Nothing to do, go idle 
			{ 
				IdleBehaviour(enemyEntity, minoComp, minoTransformComponent, enemyStats, enemyAnim);
			}
		}
		//TransformDecelerate(enemyEntity);
	}
	
	free(valueGrid);
	return true;
}