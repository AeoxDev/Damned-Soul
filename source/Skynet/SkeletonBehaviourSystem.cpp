#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "EventFunctions.h"
#include <random>



#define SKELETON_ATTACK_HITBOX 2

void ChaseBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent,
	TransformComponent* skeletonTransformComponent, StatComponent* stats, AnimationComponent* animComp, float goalDirectionX, float goalDirectionZ, bool path)
{
	if (path)
	{
		skeletonComponent->goalDirectionX = goalDirectionX;
		skeletonComponent->goalDirectionZ = goalDirectionZ;
	}
	else
	{
		skeletonComponent->goalDirectionX = playerTransformCompenent->positionX - skeletonTransformComponent->positionX;
		skeletonComponent->goalDirectionZ = playerTransformCompenent->positionZ - skeletonTransformComponent->positionZ;
	}
	//
	 //

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
	//animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);

	SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ);
	float dirX = skeletonTransformComponent->facingX, dirZ = skeletonTransformComponent->facingZ;
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

void IdleBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent, StatComponent* stats, AnimationComponent* animComp)
{

	skeletonComponent->timeCounter += GetDeltaTime();

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
	//animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);
	bool okayDirection = false;
	int limit = 128;
	while (!okayDirection)
	{
		--limit;
		if (limit < 0)
		{
			return;
		}
		if (skeletonComponent->timeCounter >= skeletonComponent->updateInterval)
		{
			skeletonComponent->timeCounter = 0.f;
			std::random_device rd;
			std::mt19937 gen(rd());
			// Define a uniform distribution for the range [-1.0, 1.0]
			std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
			float randomX = distribution(gen);
			float randomZ = distribution(gen);
			skeletonComponent->goalDirectionX = randomX;
			skeletonComponent->goalDirectionZ = randomZ;
			std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
			skeletonComponent->updateInterval = randomInterval(gen);
		}

		SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ);
		float oldX = skeletonTransformComponent->positionX;
		float oldZ = skeletonTransformComponent->positionZ;
		float bias = 1.f;

		//skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * stats->GetSpeed() * 0.5f * GetDeltaTime();
		//skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * stats->GetSpeed() * 0.5f * GetDeltaTime();
		TransformAccelerate(enemy, skeletonTransformComponent->facingX * 0.5f, skeletonTransformComponent->facingZ * 0.5f);
		if ((skeletonTransformComponent->positionX >= oldX + bias || skeletonTransformComponent->positionZ >= oldZ + bias) && skeletonTransformComponent->positionX <= oldX - bias || skeletonTransformComponent->positionZ <= oldZ - bias)
		{
			//not good direction
			skeletonTransformComponent->positionX = oldX;
			skeletonTransformComponent->positionZ = oldZ;
			skeletonComponent->timeCounter = skeletonComponent->updateInterval + 1.f;
		}
		else
		{
			// good direction
			okayDirection = true;
		}

	}
}
void CombatBehaviour(SkeletonBehaviour* sc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* stc, EntityID& ent, AnimationComponent* animComp)
{
	if (sc->attackTimer <= 0.0f)
	{
		//Increment so we don't immediately get  back in here
		sc->attackTimer += GetDeltaTime();

		//Animation setup
		animComp->aAnim = ANIMATION_ATTACK;
		animComp->aAnimTime = 0.0f;
		animComp->aAnimTimePower = 1.0f;
		animComp->aAnimTimeFactor = 3.0f; //Elliot comment: This might need to be changed when timePower changes

		float PauseThreshold = 0.3f / animComp->aAnimTimeFactor;	//When to pause the animation
		float AttackStartTime = 0.5f / enemyStats->GetAttackSpeed();//When to continue the animation
		float AttackActiveTime = AttackStartTime + 0.10f;			//When the entire attack has finished

		//Attack Telegraphing #1: Quick prep + Pause + Blink
		AddTimedEventComponentStartContinuousEnd(ent, PauseThreshold, PauseAnimation, EnemyAttackFlash, AttackStartTime, ContinueAnimation, skeleton, 1);

		//Attack Telegraphing #2: Slow prep + Gradual light
		//animComp->aAnimTimeFactor = 0.5f;
		//AddTimedEventComponentStartContinuousEnd(ent, 0.0f, nullptr, EnemyAttackGradient, 0.8f, nullptr, skeleton, 1);

		//Actual attack
		AddTimedEventComponentStartContinuousEnd(ent, AttackStartTime, EnemyBeginAttack, nullptr, AttackActiveTime, EnemyEndAttack, skeleton, 1);

		//Recovery/Daze
		float AttackTotalTime = AttackActiveTime;//When finished with the attack, become stunned
		AddTimedEventComponentStart(ent, AttackTotalTime, EnemyBecomeStunned, skeleton, 1);
	}


	/*OLD COMBAT BEHAVIOUR
	sc->attackTimer += GetDeltaTime() * animComp->aAnimTimeFactor;
	sc->goalDirectionX = ptc->positionX - stc->positionX;
	sc->goalDirectionZ = ptc->positionZ - stc->positionZ;

	//Elliot & Herman request: Make animationtime scale better for faster startup and swing.
	animComp->aAnim = ANIMATION_ATTACK;
	animComp->aAnimIdx = 0;
	//Elliot: Change in calculations for attack timer:
	animComp->aAnimTime = 0.5f * sc->attackTimer / (0.0001f + enemyStats->GetAttackSpeed());
	ANIM_BRANCHLESS(animComp);

	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (sc->attackTimer >= enemyStats->GetAttackSpeed()) // yes, we can indeed attack. 
	{
		//Niclas peaces out
		ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(ent);
		skelel->colorAdditiveBlue = skelel->baseColorAdditiveBlue;

		//Set hitbox active here.
		//Elliot's request: Add Skeleton attack hitbox instead of define
		SetHitboxActive(ent, sc->attackHitboxID, true);
		SetHitboxCanDealDamage(ent, sc->attackHitboxID, true);
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		sfx->Play(Skeleton_Attack, Channel_Base);
		RedrawUI();
		sc->attackTimer = 0.f;
		sc->attackStunDurationCounter = 0.f;
	}
	*/
}

bool SkeletonBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	SkeletonBehaviour* skeletonComponent = nullptr;
	TransformComponent* skeletonTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	AnimationComponent* enemyAnim = nullptr;
	EnemyComponent* enmComp = nullptr;
	DebuffComponent* debuff = nullptr;

	bool updateGridOnce = true;
	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));// (PathfindingMap*)MemLib::spush(sizeof(PathfindingMap));
	//*valueGrid = PathfindingMap();

	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		skeletonComponent = registry.GetComponent<SkeletonBehaviour>(enemyEntity);
		skeletonTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);
		enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);

		
		debuff = registry.GetComponent<DebuffComponent>(enemyEntity);
		if (debuff && debuff->m_frozen)
		{
			TransformDecelerate(enemyEntity);//Always decelerate
			continue; // frozen, won't do behavior stuff
		}

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
		if (skeletonComponent != nullptr && playerTransformCompenent!= nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			ML_Vector<Node> finalPath;
			skeletonComponent->updatePathCounter += GetDeltaTime();
#ifdef PATH_FINDING_VISUALIZER
			skeletonComponent->testUpdateTimer += GetDeltaTime();
#endif // TEST
			float distance = Calculate2dDistance(skeletonTransformComponent->positionX, skeletonTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			
			skeletonComponent->attackStunDurationCounter += GetDeltaTime();

			//Dazed
			if (skeletonComponent->attackStunDurationCounter <= skeletonComponent->attackStunDuration) 
			{
				// this is where we rotate the AI to avoid bullshit player tactics
				skeletonComponent->goalDirectionX = playerTransformCompenent->positionX - skeletonTransformComponent->positionX;
				skeletonComponent->goalDirectionZ = playerTransformCompenent->positionZ - skeletonTransformComponent->positionZ;
				SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ, 4.f);
			}

			//Combat
			else if (distance < skeletonComponent->meleeDistance || skeletonComponent->attackTimer > 0.0f) 
			{
				CombatBehaviour(skeletonComponent, enemyStats, playerStats, playerTransformCompenent, skeletonTransformComponent, enemyEntity, enemyAnim);
			}

			//Pathfinding
			else if (distance < 50.f) 
			{
				
#ifdef PATH_FINDING_VISUALIZER
				TransformComponent* doggoT;
				for (auto enemyEntity : View<HellhoundBehaviour, TransformComponent, StatComponent>(registry))
				{
					doggoT = registry.GetComponent<TransformComponent>(enemyEntity);
				}

#endif // TEST
				if (skeletonComponent->updatePathCounter >= skeletonComponent->updatePathLimit)
				{
					skeletonComponent->updatePathCounter = 0.f;
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
						
					finalPath = CalculateAStarPath(valueGrid, skeletonTransformComponent, playerTransformCompenent);
#ifdef PATH_FINDING_VISUALIZER
					skeletonComponent->coolVec.clear();
					skeletonComponent->counterForTest = 0;
#endif // TEST
					if (finalPath.size() > 2)
					{
						skeletonComponent->fx = finalPath[0].fx;
						skeletonComponent->fz = finalPath[0].fz;
						skeletonComponent->followPath = true;
					}
					
#ifdef PATH_FINDING_VISUALIZER
					for (int p = 0; p < finalPath.size(); p++)
					{
						finalPath[p].fx = skeletonComponent->fx;
						finalPath[p].fz = skeletonComponent->fz;
						skeletonComponent->coolVec.push_back((float)finalPath[p].x);
						skeletonComponent->coolVec.push_back((float)finalPath[p].z);
					}
#endif // TEST
					// goal (next node) - current
					if (finalPath.size() > 2 && skeletonComponent->followPath)
					{
						skeletonComponent->dirX = (float)finalPath[1].x - (float)finalPath[0].x;
						skeletonComponent->dirZ = -(float)(finalPath[1].z - (float)finalPath[0].z);
						skeletonComponent->dir2X = (float)finalPath[2].x - (float)finalPath[1].x;
						skeletonComponent->dir2Z = -(float)(finalPath[2].z - (float)finalPath[1].z);
						skeletonComponent->followPath = true;
					}
					else
					{
 						skeletonComponent->followPath = false;
					}
				}   
#ifdef PATH_FINDING_VISUALIZER
				else if(skeletonComponent->counterForTest + 1 < skeletonComponent->coolVec.size() && (skeletonComponent->testUpdateTimer >= skeletonComponent->testUpdateLimit))
				{
					skeletonComponent->testUpdateTimer = 0.f;
					Coordinate2D gridOnPos;
					GridPosition coorde;
					
					coorde.x = skeletonComponent->coolVec[skeletonComponent->counterForTest];
					coorde.z = skeletonComponent->coolVec[skeletonComponent->counterForTest + 1];
					GeometryIndependentComponent* ggg = registry.GetComponent<GeometryIndependentComponent>(playerComponent->mapID); //just need GIcomp


					coorde.fx = skeletonComponent->fx;
					coorde.fz = skeletonComponent->fz;

					gridOnPos = GridOnPosition(coorde, ggg, true);
					doggoT->positionX = gridOnPos.x;
					doggoT->positionZ = gridOnPos.z;


					skeletonComponent->counterForTest += 2;
				}
#endif // TEST
				
				if (skeletonComponent->followPath == true && skeletonComponent->updatePathCounter >= skeletonComponent->updatePathLimit / 2.f)
				{
					skeletonComponent->dirX = skeletonComponent->dir2X;
					skeletonComponent->dirZ = skeletonComponent->dir2Z;
				}
				
				if (distance < skeletonComponent->meleeDistance * 2.f) // dont follow path, go melee
				{
					skeletonComponent->followPath = false;
				} 
				ChaseBehaviour(enemyEntity, playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim, skeletonComponent->dirX, skeletonComponent->dirZ, skeletonComponent->followPath);
			}

			//Idle
			else
			{
				enmComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(enemyEntity,playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim);
			}
		}
		//Idle if there are no players on screen.
		else  if (enemyStats->GetHealth() > 0.0f)
		{
			enmComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(enemyEntity,playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim);
		}
		TransformDecelerate(enemyEntity);

		//Increment animation AFTER everything has been calculated
		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
	}

	// Pop the stack
	//MemLib::spop;
	free(valueGrid);
	return true;
}