#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Skynet\BehaviourHelper.h"
#include "EventFunctions.h"

#include <random>
#include <math.h>

void ChaseBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, MinotaurBehaviour* mc,
	TransformComponent* mtc, StatComponent* stats, AnimationComponent* enemyAnim)
{
	if (!mc->followPath)
	{
		mc->goalDirectionX = playerTransformCompenent->positionX - mtc->positionX;
		mc->goalDirectionZ = playerTransformCompenent->positionZ - mtc->positionZ;
	}
	else
	{
		mc->goalDirectionX = mc->dirX;
		mc->goalDirectionZ = mc->dirZ;
	}

	if (enemyAnim->aAnim != ANIMATION_WALK)
	{
		enemyAnim->aAnim = ANIMATION_WALK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}
	else if (enemyAnim->aAnimIdx != 0)
	{
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

	SmoothRotation(mtc, mc->goalDirectionX, mc->goalDirectionZ);
	float dirX = mtc->facingX, dirZ = mtc->facingZ;
	Normalize(dirX, dirZ);

	/*mtc->positionX += mtc->facingX * stats->GetSpeed() * GetDeltaTime();
	mtc->positionZ += mtc->facingZ * stats->GetSpeed() * GetDeltaTime();*/
	TransformAccelerate(enemy, dirX, dirZ);

	mc->jumpStunTimer += GetDeltaTime();
}

void IdleBehaviour(EntityID& enemy, MinotaurBehaviour* mc, TransformComponent* mtc, StatComponent* stats, AnimationComponent* enemyAnim)
{
	mc->timeCounter += GetDeltaTime();

	if (enemyAnim->aAnim != ANIMATION_WALK)
	{
		enemyAnim->aAnim = ANIMATION_WALK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}
	else if (enemyAnim->aAnimIdx != 0)
	{
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

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

	//mtc->positionX += mtc->facingX * stats->GetSpeed() * 0.5f * GetDeltaTime();
	//mtc->positionZ += mtc->facingZ * stats->GetSpeed() * 0.5f * GetDeltaTime();
	TransformAccelerate(enemy, mtc->facingX, mtc->facingZ);
}

void ChargeBehaviour(EntityID& enemy, TransformComponent* ptc, MinotaurBehaviour* mc, TransformComponent* mtc, StatComponent* enemyStats, EnemyComponent* enemyComp, AnimationComponent* enemyAnim)
{
	if (!mc->charging)
	{
		enemyAnim->aAnimTime = 0;

		if (enemyAnim->aAnim != ANIMATION_WALK)
		{
			enemyAnim->aAnim = ANIMATION_WALK;
			enemyAnim->aAnimIdx = 0;
			enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor * (1 + mc->charging);
		}
		else if (enemyAnim->aAnimIdx != 0)
		{
			enemyAnim->aAnimIdx = 0;
			enemyAnim->aAnimTime = 0.0f;
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

		//AddTimedEventComponentStartContinuousEnd(enemy, 0.0f, nullptr, ChargeColorFlash, mc->aimDuration - 0.2f, ResetColor);
		AddTimedEventComponentStartContinous(enemy, 0.0f, nullptr, mc->aimDuration - 0.2, ChargeColorFlash);
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
		else if (enemyAnim->aAnimIdx != 0)
		{
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

			mtc->positionX += mtc->facingX * enemyStats->GetSpeed() * 6.f * GetDeltaTime();
			mtc->positionZ += mtc->facingZ * enemyStats->GetSpeed() * 6.f * GetDeltaTime();
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

			mc->behaviourState = 1;
		}
	}
}

void JumpingBehaviour(EntityID& enemy, TransformComponent* ptc, MinotaurBehaviour* mc, TransformComponent* mtc, StatComponent* enemyStats, EnemyComponent* enemyComp, AnimationComponent* enemyAnim, PathfindingMap* valueGrid, bool& updateGridOnce)
{
	if (mc->jumpCounter < mc->jumpBreakpoint)
	{
		if (mc->jumpStunTimer <= mc->jumpStunDuration)
		{
			if (enemyAnim->aAnim != ANIMATION_IDLE)
			{
				enemyAnim->aAnim = ANIMATION_IDLE;
				enemyAnim->aAnimIdx = 1;
				enemyAnim->aAnimTime = 0.0f;
			}
			else if (enemyAnim->aAnimIdx != 1)
			{
				enemyAnim->aAnimIdx = 1;
				enemyAnim->aAnimTime = 0.0f;
			}

			mc->jumpStunTimer += GetDeltaTime();
			return;
		}
		if (mc->jumpBuildUpTimer <= mc->jumpBuildUpDuration)
		{
			if (!mc->jumping)
			{
				enemyAnim->aAnim = ANIMATION_WALK;
				enemyAnim->aAnimIdx = 1;
				enemyAnim->aAnimTime = 0.0f;

				//AddTimedEventComponentStartContinuousEnd(enemy, 0.0f, nullptr, BossBlinkBeforeShockwave, mc->jumpDuration * 0.2f, ResetColor);
				AddTimedEventComponentStartContinous(enemy, 0.0f, nullptr, mc->jumpDuration * 0.2f, BossBlinkBeforeShockwave);
				mc->jumping = true;
			}
			mc->jumpBuildUpTimer += GetDeltaTime();
		}
		else if (mc->jumpTimer <= mc->jumpDuration) // fly up in the air
		{
			if (mc->jumpTimer == 0.0f)
			{
				SetHitboxActive(enemy, 0, false);
				SetHitboxActive(enemy, 1, false);
				SetHitboxActive(enemy, 2, false);
			}
			mtc->positionY += enemyStats->GetSpeed() * 12.f * GetDeltaTime();
			mc->jumpTimer += GetDeltaTime();
		}
		else  // fly down from air
		{
			if (enemyAnim->aAnim != ANIMATION_WALK)
			{
				enemyAnim->aAnim = ANIMATION_WALK;
				enemyAnim->aAnimIdx = 2;
				enemyAnim->aAnimTime = 0.0f;
			}
			else if (enemyAnim->aAnimIdx != 2)
			{
				enemyAnim->aAnimIdx = 2;
				enemyAnim->aAnimTime = 0.0f;
			}

			if (mc->hasLandingPos == false)
			{
				if (updateGridOnce == true)
				{
					updateGridOnce = false;
					CalculateGlobalMapValuesLuciferJump(valueGrid); //generate a gridmap
					if (valueGrid->cost[0][0] == -69.f)
					{
						updateGridOnce = true; //illegal grid
						return;
					}
					TransformComponent landingPosition = FindRetreatTile(valueGrid, ptc, 10.f, 35.f); // where to land
					mc->hasLandingPos = true;
					mtc->positionX = landingPosition.positionX; //teleport in the air basically
					mtc->positionZ = landingPosition.positionZ; // happens once
					AddTimedEventComponentStart(enemy, 0.0f, CreateLandingIndicator, EnemyType::minotaur);
				}
			}
			if (mtc->positionY >= 0.f) // still in the air
			{
				mtc->positionY -= enemyStats->GetSpeed() * 10.f * GetDeltaTime();
			}
			else // on ground or below ground, set to ground and SHOCKWAVE with timed event
			{
				if (mc->jumping == true) // make sure we only reset values and create a shockwave once
				{
					//reset variables to beheaviour. No more jump, get dazed
					mc->jumping = false;
					updateGridOnce = true;
					mc->hasLandingPos = false;
					mc->jumpStunTimer = 0;
					mc->jumpBuildUpTimer = 0;
					mc->jumpTimer = 0;

					//minoComp->isDazed = true;
					SetHitboxActive(enemy, 0, true);
					SetHitboxActive(enemy, 1, true);
					SetHitboxActive(enemy, 2, true);
					mtc->positionY = 0.f;

					//shockwave here
					AddTimedEventComponentStartContinuousEnd(enemy, 0.0f, BossShockwaveStart, BossShockwaveExpand, 1.0f, BossShockwaveEnd, 0, 1);
					mc->jumpCounter++;
				}
			}
		}
	}
	else
	{
		enemyAnim->aAnim = ANIMATION_IDLE;
		enemyAnim->aAnimIdx = 1;
		enemyAnim->aAnimTime = 0.0f;

		mc->attackStunTimer = 0;
		mc->behaviourState = 0;
		mc->jumpCounter = 0;

		//Calculate new teleport breakpoint
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [2.0, 4.0]
		std::uniform_real_distribution<float> distribution(2.0f, 4.0f);
		mc->jumpBreakpoint = (int)distribution(gen);
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
			ML_Vector<Node> finalPath;
			minoComp->updatePathCounter += GetDeltaTime();

			float distance = Calculate2dDistance(minoTransformComponent->positionX, minoTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);

			if (minoComp->hasSeenPlayer != true)
			{
				if (distance < 60.f)
				{
					minoComp->hasSeenPlayer = true;
				}
			}


			if (minoComp->attackStunTimer <= minoComp->attackStunDuration)
			{
				if (enemyAnim->aAnim != ANIMATION_IDLE)
				{
					enemyAnim->aAnim = ANIMATION_IDLE;
					enemyAnim->aAnimIdx = 1;
					enemyAnim->aAnimTime = 0.0f;
				}
				else if (enemyAnim->aAnimIdx != 1)
				{
					enemyAnim->aAnimIdx = 1;
					enemyAnim->aAnimTime = 0.0f;
				}

				minoComp->attackStunTimer += GetDeltaTime();
			}
			else if (minoComp->jumping || (minoComp->behaviourState == 1 && distance < 25.0f)) // jump
			{
				JumpingBehaviour(enemyEntity, playerTransformCompenent, minoComp, minoTransformComponent, enemyStats, enmComp, enemyAnim, valueGrid, updateGridOnce);
			}
			else if (minoComp->charging || (minoComp->behaviourState == 0 && distance < 40.0f))
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
			else if (minoTransformComponent->positionY < 0.05f)
			{
				if (minoComp->updatePathCounter >= minoComp->updatePathLimit)
				{
					minoComp->updatePathCounter = 0.f;
					if (playerComponent != nullptr && updateGridOnce)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesSkeleton(valueGrid, playerTransformCompenent);
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true;
							continue;
						}
					}
 					finalPath = CalculateAStarPath(valueGrid, minoTransformComponent, playerTransformCompenent);

					// goal (next node) - current
					if (finalPath.size() > 2)
					{
						minoComp->dirX = (float)finalPath[1].x - (float)finalPath[0].x;
						minoComp->dirZ = -(float)(finalPath[1].z - (float)finalPath[0].z);
						minoComp->dir2X = (float)finalPath[2].x - (float)finalPath[1].x;
						minoComp->dir2Z = -(float)(finalPath[2].z - (float)finalPath[1].z);
						minoComp->followPath = true;
					}
					else
					{
						minoComp->followPath = false;
					}
				}
				
				if (minoComp->followPath == true && minoComp->updatePathCounter >= minoComp->updatePathLimit / 2.f)
				{
					minoComp->dirX = minoComp->dir2X;
					minoComp->dirZ = minoComp->dir2Z;
				}

				if(minoComp->hasSeenPlayer == true)
					ChaseBehaviour(enemyEntity, playerComponent, playerTransformCompenent, minoComp, minoTransformComponent, enemyStats, enemyAnim);
				else // Go idle 
					IdleBehaviour(enemyEntity, minoComp, minoTransformComponent, enemyStats, enemyAnim);
			}
		}

		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
		ANIM_BRANCHLESS(enemyAnim);
		TransformDecelerate(enemyEntity);
	}
	
	free(valueGrid);
	return true;
}