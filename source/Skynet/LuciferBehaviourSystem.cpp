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
#include "EventFunctions.h"


void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, LuciferBehaviour* lc, TransformComponent* ltc, StatComponent* enemyStats, AnimationComponent* enemyAnim, float goalDirectionX, float goalDirectionZ, bool path, bool move)
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


	
	if (move)
	{
		ltc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
		ltc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
	}
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


	for (auto enemyEntity : View<LuciferBehaviour, TransformComponent, StatComponent, AnimationComponent>(registry))
	{
		luciferComponent = registry.GetComponent<LuciferBehaviour>(enemyEntity);
		luciferTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
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
		if (luciferTransformComponent != nullptr && playerTransformCompenent != nullptr && enemyAnim != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			ML_Vector<Node> finalPath;
			luciferComponent->updatePathCounter += GetDeltaTime();
			
			float distance = Calculate2dDistance(luciferTransformComponent->positionX, luciferTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			luciferComponent->attackStunDurationCounter += GetDeltaTime();


			//charging?
			if (luciferComponent->isChargeCharge)
			{
				luciferComponent->chargeBehevCounter += GetDeltaTime(); //just count charge duration stuff
			}

			

			//time to stop charging?
			if (luciferComponent->chargeBehevCounter >= luciferComponent->chargeBehevCounterTiming || enemyStats->GetHealth() <= luciferComponent->limitHP)
			{
				luciferComponent->isChargeCharge = false;
				luciferComponent->chargeBehevCounter = 0.f;
				luciferComponent->nextSpecialIsSpawn = true;
				luciferComponent->isDazed = true;
				luciferComponent->limitHP = 0.f;
			}

			// are we dazed? dazed means standing still like an idiot
			if (luciferComponent->isDazed)
			{
				luciferComponent->dazeCounter += GetDeltaTime();
				if (luciferComponent->dazeCounter >= luciferComponent->dazeTimeAmount)
				{
					luciferComponent->isDazed = false;
					luciferComponent->dazeCounter = 0.f;
				}
				else
				{
					SetHitboxActive(enemyEntity, luciferComponent->attackHitboxID, false); // to not make player rage
					SetHitboxCanDealDamage(enemyEntity, luciferComponent->attackHitboxID, false);
					continue; // skip, do nothing. Just stand still and be dazed
					//maybe play dazed anymation in this scope?
				}
			}

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

			if (luciferComponent->nextSpecialIsSpawn) // SPAWN ENEMIES
			{
				luciferComponent->nextSpecialIsSpawn = false;
				//spawn an ice enemy
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
					TransformComponent tran = FindSpawnTile(valueGrid, luciferTransformComponent, 8.f, 60.f);
					SetupEnemy(EnemyType::frozenHellhound, tran.positionX, 0.f, tran.positionZ); 
					tran = FindSpawnTile(valueGrid, luciferTransformComponent, 8.f, 60.f);
					SetupEnemy(EnemyType::frozenEye, tran.positionX, 0.f, tran.positionZ);
					tran = FindSpawnTile(valueGrid, luciferTransformComponent, 8.f, 60.f);
					SetupEnemy(EnemyType::frozenImp, tran.positionX, 0.f, tran.positionZ);
				}
				continue;
			}
			else if (luciferComponent->isChargeCharge  == false && luciferComponent->isJumpJump == false)
			{
				luciferComponent->chargeBehevCounter = 0.f;
				int whichAttack = rand() % 10 + 1; // 1 - 10
				if (whichAttack > 3 ) //  charge run
				{
					luciferComponent->isChargeCharge = true;
					// runs towards the player dealing
					// damage if player is close.
					// Ends after 10 seconds or
					// if taken enough damage

					float percent = enemyStats->GetMaxHealth() / 5.f;
					luciferComponent->limitHP = (float)enemyStats->GetMaxHealth();
					while (luciferComponent->limitHP >= enemyStats->GetHealth())
					{
						luciferComponent->limitHP = luciferComponent->limitHP - percent;
					}
					
				}
				else // jump jump
				{
					luciferComponent->isJumpJump = true;
					luciferComponent->flyCounter = 0.f;
					luciferComponent->hasLandingPos = false;
					// jumps out of the playspace and lands
					// in a safe spot close to the player and 
					// sends out a large shockwave 

					//registry.RemoveComponent<EnemyComponent>(enemyEntity); // this makes it immune to damage
					//SetHitboxCanTakeDamage(enemyEntity, luciferComponent->hitBoxID, false);
					SetHitboxActive(enemyEntity, 0, false);
					SetHitboxActive(enemyEntity, 1, false);
					SetHitboxActive(enemyEntity, 2, false);
				}
				//after either one of these, the boss goes into a daze and then summons enemies!
			}
			else if (luciferComponent->isJumpJump) // this is the fly up in air thing. jump jump
			{
				luciferComponent->flyCounter += GetDeltaTime();
				if (luciferComponent->flyCounter <= luciferComponent->flyTime) // fly up in the air
				{
					luciferTransformComponent->positionY += enemyStats->GetSpeed() * 7.f * GetDeltaTime();
				}
				else  // fly down from air
				{
					if (playerComponent != nullptr && updateGridOnce && luciferComponent->hasLandingPos == false)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesLuciferJump(valueGrid); //generate a gridmap
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true; //illegal grid
							continue;
						}
						TransformComponent landingPosition = FindRetreatTile(valueGrid, playerTransformCompenent, 10.f, 20.f); // where to land
						luciferComponent->hasLandingPos = true;
						luciferTransformComponent->positionX = landingPosition.positionX; //teleport in the air basically
						luciferTransformComponent->positionZ = landingPosition.positionZ; // happens once
					}
					if (luciferTransformComponent->positionY > 0.f) // still in the air
					{
						luciferTransformComponent->positionY -= enemyStats->GetSpeed() * 12.f * GetDeltaTime();
					}
					else // on ground or below ground, set to ground and SHOCKWAVE with timed event
					{
						//reset variables to beheaviour. No more jump, get dazed
						luciferComponent->isJumpJump = false;
						luciferComponent->isDazed = true;
						SetHitboxActive(enemyEntity, 0, true);
						SetHitboxActive(enemyEntity, 1, true);
						SetHitboxActive(enemyEntity, 2, true);
						luciferTransformComponent->positionY = 0.f;
						luciferComponent->nextSpecialIsSpawn = true;


						//shockwave here
						AddTimedEventComponentStartContinuousEnd(enemyEntity, 0.0f, BossShockwaveStart, BossShockwaveExpand, 4.0f, BossShockwaveEnd, 0, 1);
					}
				}
			}
			else if (distance < luciferComponent->meleeDistance || luciferComponent->attackTimer > 0.0f) // melee attack
			{
				CombatBehaviour(luciferComponent, enemyStats, playerStats, playerTransformCompenent, luciferTransformComponent, enemyEntity, enemyAnim);
			}
			else if(luciferComponent->isChargeCharge) // chase behaviour or jump jump
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
							updateGridOnce = true; //illegal grid
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

				ChaseBehaviour(playerComponent, playerTransformCompenent, luciferComponent, luciferTransformComponent, enemyStats, 
					enemyAnim, luciferComponent->dirX, luciferComponent->dirZ, luciferComponent->followPath, true);
			}



		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			if(enmComp != nullptr)
			enmComp->lastPlayer.index = -1;//Search for a new player to hit.
			//IdleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim);
		}
		TransformDecelerate(enemyEntity);
	}

	// pop the value map
	//MemLib::spop();
	free(valueGrid);
	return true;
}