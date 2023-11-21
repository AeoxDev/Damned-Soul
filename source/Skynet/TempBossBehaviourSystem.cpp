#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Hitbox.h"



void ChaseBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, TempBossBehaviour* tempBossComponent,
	TransformComponent* tempBossTransformComponent, StatComponent* stats, AnimationComponent* animComp, float goalDirectionX, float goalDirectionZ, bool path) 
{
	if (path)
	{
		tempBossComponent->goalDirectionX = goalDirectionX;
		tempBossComponent->goalDirectionZ = goalDirectionZ;
	}
	else
	{
		tempBossComponent->goalDirectionX = playerTransformCompenent->positionX - tempBossTransformComponent->positionX;
		tempBossComponent->goalDirectionZ = playerTransformCompenent->positionZ - tempBossTransformComponent->positionZ;
	}
	//
	 //

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
	animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);

	SmoothRotation(tempBossTransformComponent, tempBossComponent->goalDirectionX, tempBossComponent->goalDirectionZ);
	float dirX = tempBossTransformComponent->facingX, dirZ = tempBossTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	//skeletonTransformComponent->positionX += dirX * stats->GetSpeed() * GetDeltaTime();
	//skeletonTransformComponent->positionZ += dirZ * stats->GetSpeed() * GetDeltaTime();
	TransformAccelerate(enemy, dirX, dirZ);
}

void IdleBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, TempBossBehaviour* tempBossComponent, TransformComponent* tempBossTransformComponent, StatComponent* stats, AnimationComponent* animComp)
{
	tempBossComponent->timeCounter += GetDeltaTime();

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
	animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);
	bool okayDirection = false;
	while (!okayDirection)
	{
		if (tempBossComponent->timeCounter >= tempBossComponent->updateInterval)
		{
			tempBossComponent->timeCounter = 0.f;
			std::random_device rd;
			std::mt19937 gen(rd());
			// Define a uniform distribution for the range [-1.0, 1.0]
			std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
			float randomX = distribution(gen);
			float randomZ = distribution(gen);
			tempBossComponent->goalDirectionX = randomX;
			tempBossComponent->goalDirectionZ = randomZ;
			std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
			tempBossComponent->updateInterval = randomInterval(gen);
		}

		SmoothRotation(tempBossTransformComponent, tempBossComponent->goalDirectionX, tempBossComponent->goalDirectionZ);
		float oldX = tempBossTransformComponent->positionX;
		float oldZ = tempBossTransformComponent->positionZ;
		float bias = 1.f;

		//skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * stats->GetSpeed() * 0.5f * GetDeltaTime();
		//skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * stats->GetSpeed() * 0.5f * GetDeltaTime();
		TransformAccelerate(enemy, tempBossTransformComponent->facingX * 0.5f, tempBossTransformComponent->facingZ * 0.5f);
		if ((tempBossTransformComponent->positionX >= oldX + bias || tempBossTransformComponent->positionZ >= oldZ + bias) && tempBossTransformComponent->positionX <= oldX - bias || tempBossTransformComponent->positionZ <= oldZ - bias)
		{
			//not good direction
			tempBossTransformComponent->positionX = oldX;
			tempBossTransformComponent->positionZ = oldZ;
			tempBossComponent->timeCounter = tempBossComponent->updateInterval + 1.f;
		}
		else
		{
			// good direction
			okayDirection = true;
		}

	}
}

//void TemporaryPretendAnimation()
//{
//	// lmao funny code do many many things
//	int i = 0;
//}
//void BeginHitting(EntityID& entity)
//{
//	auto comp = registry.GetComponent<EnemyComponent>(entity);
//	SetHitboxActive(entity, comp->attackHitBoxID, true);
//	SetHitboxCanDealDamage(entity, comp->attackHitBoxID, true);
//}
//void WeShallOverCome(EntityID& entity)
//{
//	auto comp = registry.GetComponent<EnemyComponent>(entity);
//	SetHitboxActive(entity, comp->attackHitBoxID, false);
//	SetHitboxCanDealDamage(entity, comp->attackHitBoxID, false);
//}


void CombatBehaviour(TempBossBehaviour* bc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* btc, EnemyComponent* enmComp, EntityID& ent, AnimationComponent* animComp)
{
	bc->attackTimer += GetDeltaTime() * animComp->aAnimTimeFactor;
	bc->goalDirectionX = ptc->positionX - btc->positionX;
	bc->goalDirectionZ = ptc->positionZ - btc->positionZ;

	//Elliot & Herman request: Make animationtime scale better for faster startup and swing.
	animComp->aAnim = ANIMATION_ATTACK;
	animComp->aAnimIdx = 0;
	//Elliot: Change in calculations for attack timer:
	animComp->aAnimTime = 0.5f * bc->attackTimer / (0.0001f + enemyStats->GetAttackSpeed());
	ANIM_BRANCHLESS(animComp);

	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (bc->attackTimer >= enemyStats->GetAttackSpeed()) // yes, we can indeed attack. 
	{
		//Set hitbox active here.
		//Elliot's request: Add Skeleton attack hitbox instead of define
		SetHitboxActive(ent, enmComp->attackHitBoxID, true);
		SetHitboxCanDealDamage(ent, enmComp->attackHitBoxID, true);
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		sfx->Play(Skeleton_Attack, Channel_Base);
		RedrawUI();
		bc->attackTimer = 0.f;
		bc->attackStunDurationCounter = 0.f;
	}
}

bool TempBossBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	TempBossBehaviour* tempBossComponent = nullptr;
	TransformComponent* tempBossTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enmComp = nullptr;
	AnimationComponent* enemyAnim = nullptr;

	bool updateGridOnce = true;
	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));

	for (auto enemyEntity : View<TempBossBehaviour, TransformComponent, StatComponent>(registry))
	{
		tempBossComponent = registry.GetComponent<TempBossBehaviour>(enemyEntity);
		tempBossTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

		//Find a player to kill.
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
					if (enemyEntity.index == playerEntity.index)
					{
						continue;
					}
					enmComp->lastPlayer.index = -1;
				}
			}
		}
		if (tempBossComponent != nullptr && playerTransformCompenent != nullptr && enmComp != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(tempBossTransformComponent->positionX, tempBossTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			ML_Vector<Node> finalPath;
			tempBossComponent->updatePathCounter += GetDeltaTime();
			tempBossComponent->attackStunDurationCounter += GetDeltaTime();

			if (tempBossComponent->attackStunDurationCounter <= tempBossComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
				//Elliot: When finished, reset attack timer and hitbox
				tempBossComponent->attackTimer = 0.0f;
				enemyAnim->aAnimTime += (float)(enemyAnim->aAnimTime < 1.0f) * GetDeltaTime();
				if (enemyAnim->aAnimTime > 0.8f)
				{
					SetHitboxActive(enemyEntity, enmComp->attackHitBoxID, false);
					SetHitboxCanDealDamage(enemyEntity, enmComp->attackHitBoxID, false);
				}
				//enemyAnim->aAnimTime += (float)(enemyAnim->aAnimTime < 1.0f) * GetDeltaTime();
				continue;
			}
			else//Elliot: Turn off attack hitbox to not make player rage.
			{
				SetHitboxActive(enemyEntity, enmComp->attackHitBoxID, false);
				SetHitboxCanDealDamage(enemyEntity, enmComp->attackHitBoxID, false);
			}

			//Elliot: If in attack, keep attacking even if player is outside
			if (distance < tempBossComponent->meleeDistance || tempBossComponent->attackTimer > 0.0f)
			{
				CombatBehaviour(tempBossComponent, enemyStats, playerStats, playerTransformCompenent, tempBossTransformComponent,enmComp, enemyEntity, enemyAnim);
			}
			else if (distance < 70) //hunting distance
			{
				if (tempBossComponent->updatePathCounter >= tempBossComponent->updatePathLimit)
				{
					tempBossComponent->updatePathCounter = 0.f;
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

					finalPath = CalculateAStarPath(valueGrid, tempBossTransformComponent, playerTransformCompenent);

					if (finalPath.size() > 2)
					{
						tempBossComponent->fx = finalPath[0].fx;
						tempBossComponent->fz = finalPath[0].fz;
						tempBossComponent->followPath = true;
					}
					// goal (next node) - current
					if (finalPath.size() > 2 && tempBossComponent->followPath)
					{
						tempBossComponent->dirX = (float)finalPath[1].x - (float)finalPath[0].x;
						tempBossComponent->dirZ = -(float)(finalPath[1].z - (float)finalPath[0].z);
						tempBossComponent->dir2X = (float)finalPath[2].x - (float)finalPath[1].x;
						tempBossComponent->dir2Z = -(float)(finalPath[2].z - (float)finalPath[1].z);
						tempBossComponent->followPath = true;
					}
					else
					{
						tempBossComponent->followPath = false;
					}
				}
				if (tempBossComponent->followPath == true && tempBossComponent->updatePathCounter >= tempBossComponent->updatePathLimit / 2.f)
				{
					tempBossComponent->dirX = tempBossComponent->dir2X;
					tempBossComponent->dirZ = tempBossComponent->dir2Z;
				}

				if (distance < tempBossComponent->meleeDistance * 2.f) // dont follow path, go melee
				{
					tempBossComponent->followPath = false;
				}
				ChaseBehaviour(enemyEntity, playerComponent, playerTransformCompenent, tempBossComponent, tempBossTransformComponent, enemyStats, enemyAnim, tempBossComponent->dirX, tempBossComponent->dirZ, tempBossComponent->followPath);

			}
			else // idle
			{
				enmComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, tempBossComponent, tempBossTransformComponent, enemyStats, enemyAnim);
			}
		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enmComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, tempBossComponent, tempBossTransformComponent, enemyStats, enemyAnim);
		}
		TransformDecelerate(enemyEntity);
	}

	free(valueGrid);
	return true;
}