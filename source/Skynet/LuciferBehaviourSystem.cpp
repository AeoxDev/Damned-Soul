#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Skynet\BehaviourHelper.h"
#include "ParticleComponent.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"



void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, LuciferBehaviour* lc, TransformComponent* ltc, StatComponent* enemyStats, AnimationComponent* enemyAnim, float goalDirectionX, float goalDirectionZ, bool path)
{
	if (path)
	{
		lc->goalDirectionX = goalDirectionX;
		lc->goalDirectionZ = goalDirectionZ;
	}
	else
	{
		lc->goalDirectionX = playerTransformCompenent->positionX - ltc->positionX;
		lc->goalDirectionZ = playerTransformCompenent->positionZ - ltc->positionZ;
	}




	lc->goalDirectionX = playerTransformCompenent->positionX - ltc->positionX;
	lc->goalDirectionZ = playerTransformCompenent->positionZ - ltc->positionZ;

	SmoothRotation(ltc, lc->goalDirectionX, lc->goalDirectionZ, 35.f);
	float dirX = ltc->facingX, dirZ = ltc->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}


	
	
	ltc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
	ltc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
}



void CombatBehaviour(LuciferBehaviour* sc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* ltc, EntityID& ent, AnimationComponent* animComp)
{
	sc->attackTimer += GetDeltaTime();// *animComp->aAnimTimeFactor;
	sc->goalDirectionX = ptc->positionX - ltc->positionX;
	sc->goalDirectionZ = ptc->positionZ - ltc->positionZ;


	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (sc->attackTimer >= enemyStats->GetAttackSpeed()) // yes, we can indeed attack. 
	{
		//Set hitbox active here.
		//Elliot's request: Add Skeleton attack hitbox instead of define
		SetHitboxActive(ent, sc->attackHitboxID, true);
		SetHitboxCanDealDamage(ent, sc->attackHitboxID, true);
		/*SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		sfx->Play(Skeleton_Attack, Channel_Base);*/
		RedrawUI();
		sc->attackTimer = 0.f;
		sc->attackStunDurationCounter = 0.f;
	}
}



bool LuciferBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	LuciferBehaviour* luciferComponent = nullptr;
	TransformComponent* luciferTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enmComp = nullptr;
	AnimationComponent* enemyAnim = nullptr;

	bool updateGridOnce = true;

	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));//(PathfindingMap*)MemLib::spush(sizeof(PathfindingMap));


	for (auto enemyEntity : View<LuciferBehaviour, TransformComponent, StatComponent, EnemyComponent, AnimationComponent>(registry))
	{
		luciferComponent = registry.GetComponent<LuciferBehaviour>(enemyEntity);
		luciferTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
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
					enmComp->lastPlayer.index = -1;
				}
			}
		}

		//ACTUAL BEHAVIOUR
		if (luciferTransformComponent != nullptr && playerTransformCompenent != nullptr && enmComp != nullptr && enemyAnim != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			ML_Vector<Node> finalPath;
			luciferComponent->updatePathCounter += GetDeltaTime();
			luciferComponent->spawnCounter += GetDeltaTime();
			float distance = Calculate2dDistance(luciferTransformComponent->positionX, luciferTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			luciferComponent->attackStunDurationCounter += GetDeltaTime();

			if (luciferComponent->attackStunDurationCounter <= luciferComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
				//Elliot: When finished, reset attack timer and hitbox
				luciferComponent->attackTimer = 0.0f;
				//enemyAnim->aAnimTime += (float)(enemyAnim->aAnimTime < 1.0f) * GetDeltaTime();
				//Turn yellow for opening:

				continue;
			}
			else//Elliot: Turn off attack hitbox to not make player rage.
			{
				SetHitboxActive(enemyEntity, luciferComponent->attackHitboxID, false);
				SetHitboxCanDealDamage(enemyEntity, luciferComponent->attackHitboxID, false);
			}



			if (luciferComponent->spawnCounter >= luciferComponent->spawnTiming) // SPAWN ENEMIES
			{
				//spawn an ice enemy
				luciferComponent->spawnCounter = 0.f;
				int levelOfDamage = 0;

				if (enemyStats->GetHealth() > enemyStats->GetMaxHealth() / 3.f * 2.f) //least hurt, little power
				{
					levelOfDamage = 1;
				}
				else if (enemyStats->GetHealth() > enemyStats->GetMaxHealth() / 3.f) //middle hurt, middle power
				{
					levelOfDamage = 2;
				}
				else  //most hurt, more power
				{
					levelOfDamage = 3;
				}
				CalculateGlobalMapValuesHellhound(valueGrid);
				for (int i = 0; i < levelOfDamage; i++)
				{
					TransformComponent tran = FindRetreatTile(valueGrid, luciferTransformComponent, 8.f, 60.f);
					SetupEnemy(EnemyType::frozenHellhound, tran.positionX, 0.f, tran.positionZ); 
				}
			}
			else if (distance < luciferComponent->meleeDistance || luciferComponent->attackTimer > 0.0f) // melee attack
			{
				CombatBehaviour(luciferComponent, enemyStats, playerStats, playerTransformCompenent, luciferTransformComponent, enemyEntity, enemyAnim);
			}
			else // chase behaviour
			{
				if (luciferComponent->updatePathCounter >= luciferComponent->updatePathLimit)
				{
					luciferComponent->updatePathCounter = 0;
					if (playerComponent != nullptr && updateGridOnce)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesHellhound(valueGrid);
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true;
							continue;
						}
					}

					finalPath = CalculateAStarPath(valueGrid, luciferTransformComponent, playerTransformCompenent);

					if (finalPath.size() > 2)
					{
						luciferComponent->fx = finalPath[0].fx;
						luciferComponent->fz = finalPath[0].fz;
						luciferComponent->followPath = true;
					}

					// goal (next node) - current
					if (finalPath.size() > 2 && luciferComponent->followPath)
					{
						luciferComponent->dirX = (float)finalPath[1].x - (float)finalPath[0].x;
						luciferComponent->dirZ = -(float)(finalPath[1].z - (float)finalPath[0].z);
						luciferComponent->dir2X = (float)finalPath[2].x - (float)finalPath[1].x;
						luciferComponent->dir2Z = -(float)(finalPath[2].z - (float)finalPath[1].z);
						luciferComponent->followPath = true;
					}
					else
					{
						luciferComponent->followPath = false;
					}
				}

				ChaseBehaviour(playerComponent, playerTransformCompenent, luciferComponent, luciferTransformComponent, enemyStats, enemyAnim, luciferComponent->dirX, luciferComponent->dirZ, luciferComponent->followPath);
			}



		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enmComp->lastPlayer.index = -1;//Search for a new player to hit.
			//IdleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim);
		}

	}

	// pop the value map
	//MemLib::spop();
	free(valueGrid);
	return true;
}