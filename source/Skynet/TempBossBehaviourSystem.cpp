#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Hitbox.h"
#include "EventFunctions.h"


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
	ANIM_BRANCHLESS(animComp);

	SmoothRotation(tempBossTransformComponent, tempBossComponent->goalDirectionX, tempBossComponent->goalDirectionZ);
	float dirX = tempBossTransformComponent->facingX, dirZ = tempBossTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	//tempBossTransformComponent->positionX += dirX * stats->GetSpeed() * GetDeltaTime();
	//tempBossTransformComponent->positionZ += dirZ * stats->GetSpeed() * GetDeltaTime();
	TransformAccelerate(enemy, dirX, dirZ);
}

void IdleBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, TempBossBehaviour* tempBossComponent, TransformComponent* tempBossTransformComponent, StatComponent* stats, AnimationComponent* animComp)
{
	tempBossComponent->timeCounter += GetDeltaTime();

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
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


void CombatBehaviour(TempBossBehaviour* bc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* btc, EnemyComponent* enmComp, EntityID& ent, AnimationComponent* animComp)
{
	if (bc->attackTimer <= 0.0f) // yes, we can indeed attack. (bc->attackTimer >= enemyStats->GetAttackSpeed())
	{
		bc->isAttacking = true;
		//Increment so we don't immediately get  back in here
		bc->attackTimer += GetDeltaTime();

		//Animation setup
		animComp->aAnim = ANIMATION_ATTACK;
		animComp->aAnimTime = 0.0f;
		animComp->aAnimTimePower = 1.0f;
		animComp->aAnimTimeFactor = 3.0f; //Elliot comment: This might need to be changed when timePower changes

		float PauseThreshold = 0.3f / animComp->aAnimTimeFactor;	//When to pause the animation
		float AttackStartTime = 0.3f / enemyStats->GetAttackSpeed();//When to continue the animation
		float AttackActiveTime = AttackStartTime + 0.10f;			//When the entire attack has finished

		//Attack Telegraphing #1: Quick prep + Pause + Blink
		AddTimedEventComponentStartContinuousEnd(ent, PauseThreshold, PauseAnimation, EnemyAttackFlash, AttackStartTime, ContinueAnimation, EnemyType::tempBoss, 1);

		//Attack Telegraphing #2: Slow prep + Gradual light
		//animComp->aAnimTimeFactor = 0.5f;
		//AddTimedEventComponentStartContinuousEnd(ent, 0.0f, nullptr, EnemyAttackGradient, 0.8f, nullptr, skeleton, 1);

		//Actual attack
		AddTimedEventComponentStartContinuousEnd(ent, AttackStartTime, EnemyBeginAttack, nullptr, AttackActiveTime, EnemyEndAttack, EnemyType::tempBoss, 1);

		//Recovery/Daze
		float AttackTotalTime = AttackActiveTime;//When finished with the attack, become stunned
		AddTimedEventComponentStart(ent, AttackTotalTime, EnemyBecomeStunned, EnemyType::tempBoss, 1);
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
			tempBossComponent->shockwaveChanceCounter += GetDeltaTime();

			if (distance < 70.f && tempBossComponent->isDazed == false && tempBossComponent->willDoShockWave == false
				&& tempBossComponent->attackStunDurationCounter > tempBossComponent->attackStunDuration && tempBossComponent->isAttacking == false)
			{
				if (tempBossComponent->shockwaveChanceCounter >= tempBossComponent->shockwaveChanceInterval)
				{
					tempBossComponent->shockwaveChanceCounter = 0.f;
					int odds = rand() % 100 + 1; // 1 - 100
					if (odds <= tempBossComponent->shockwaveOdds)
					{
						// SHOCKWAVE BABY
						tempBossComponent->willDoShockWave = true;
					}

					//well, nothing happened....maybe next time

				}
			}

			//Dazed specifically because of shockwave, do not touch IN CAPS
			if (tempBossComponent->isDazed)
			{
				TransformDecelerate(enemyEntity);
				tempBossComponent->dazeCounter += GetDeltaTime();
				if (tempBossComponent->dazeCounter >= tempBossComponent->dazeTime)
				{
					tempBossComponent->shockwaveChanceCounter = 0.f;
					tempBossComponent->shockwaveChargeCounter = 0.f;
					tempBossComponent->dazeCounter = 0.f;
					tempBossComponent->isDazed = false;
				}
				continue;
			}

			//Dazed
			if (tempBossComponent->attackStunDurationCounter <= tempBossComponent->attackStunDuration)
			{
				// this is where we rotate the AI to avoid bullshit player tactics
				tempBossComponent->goalDirectionX = playerTransformCompenent->positionX - tempBossTransformComponent->positionX;
				tempBossComponent->goalDirectionZ = playerTransformCompenent->positionZ - tempBossTransformComponent->positionZ;
				SmoothRotation(tempBossTransformComponent, tempBossComponent->goalDirectionX, tempBossComponent->goalDirectionZ, 4.f);
			}

			//Combat
			else if ((distance < tempBossComponent->meleeDistance || tempBossComponent->attackTimer > 0.0f) && tempBossComponent->willDoShockWave == false)
			{
				CombatBehaviour(tempBossComponent, enemyStats, playerStats, playerTransformCompenent, tempBossTransformComponent,enmComp, enemyEntity, enemyAnim);
			}

			//Pathfinding
			else if (distance < 70) 
			{
				// insert chance for shockwave. 
				
				if (tempBossComponent->willDoShockWave) // charge then do it!
				{
					tempBossComponent->shockwaveChanceCounter = 0.f;
					tempBossComponent->shockwaveChargeCounter += GetDeltaTime();

					if (tempBossComponent->shockwaveChargeCounter >= tempBossComponent->shockWaveChargeCooldown * 0.8f && tempBossComponent->isBlinking == false)
					{
						AddTimedEventComponentStartContinuousEnd(enemyEntity, 0.0f, nullptr, BossBlinkBeforeShockwave, tempBossComponent->shockWaveChargeCooldown * 0.2f, BossResetBeforeShockwave);
					}
					else if (tempBossComponent->shockwaveChargeCounter >= tempBossComponent->shockWaveChargeCooldown) // Dew it..
					{
						TransformDecelerate(enemyEntity);
						tempBossComponent->isDazed = true;
						tempBossComponent->willDoShockWave = false;
						AddTimedEventComponentStartContinuousEnd(enemyEntity, 0.0f, BossShockwaveStart, BossShockwaveExpand, tempBossComponent->dazeTime, BossShockwaveEnd, 0, 1);
						registry.AddComponent<ParticleComponent>(enemyEntity, tempBossComponent->dazeTime, 500.f, 0.5f, 0.f, 0.f, 0.f, 30.f, 2000, ComputeShaders::PULSE);
						//30.f is what is growthspeed in bossshockwaveexpand
					}

					TransformDecelerate(enemyEntity);
					continue; // dont chase
				}

				if (tempBossComponent->updatePathCounter >= tempBossComponent->updatePathLimit)
				{
					tempBossComponent->updatePathCounter = 0.f;
					if (playerComponent != nullptr && updateGridOnce)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesHellhound(valueGrid);
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true;
							TransformDecelerate(enemyEntity);
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

		//Increment animation AFTER everything has been calculated
		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
	}

	free(valueGrid);
	return true;
}