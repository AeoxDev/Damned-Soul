#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>


#define SKELETON_ATTACK_HITBOX 2

void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, 
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
	animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);

	SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ);
	float dirX = skeletonTransformComponent->facingX, dirZ = skeletonTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	skeletonTransformComponent->positionX += dirX * stats->GetSpeed() * GetDeltaTime();
	skeletonTransformComponent->positionZ += dirZ * stats->GetSpeed() * GetDeltaTime();
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent, StatComponent* stats, AnimationComponent* animComp)
{
	skeletonComponent->timeCounter += GetDeltaTime();

	animComp->aAnim = ANIMATION_IDLE;
	animComp->aAnimIdx = 0;
	animComp->aAnimTime += GetDeltaTime() * animComp->aAnimTimeFactor;
	ANIM_BRANCHLESS(animComp);

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


	skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * stats->GetSpeed() / 2.f * GetDeltaTime();
	skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * stats->GetSpeed() / 2.f * GetDeltaTime();

}
void CombatBehaviour(SkeletonBehaviour* sc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* stc, EntityID& ent, AnimationComponent* animComp)
{
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
		//Set hitbox active here.
		//Elliot's request: Add Skeleton attack hitbox instead of define
		SetHitboxActive(ent, sc->attackHitboxID, true);
		SetHitboxCanDealDamage(ent, sc->attackHitboxID, true);
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		sfx->Play(Skeleton_Attack, Channel_Base);
		RedrawUI();
		sc->attackTimer = 0;
		sc->attackStunDurationCounter = 0;
	}
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

	for (auto playerEntity : View<PlayerComponent, TransformComponent, StatComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
		playerStats = registry.GetComponent< StatComponent>(playerEntity);
	}
	PathfindingMap valueGrid;
	
	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent, StatComponent>(registry))
	{
		if (playerComponent != nullptr)
			valueGrid = CalculateGlobalMapValuesSkeleton(playerComponent->mapID);
		continue;
	}
	

	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent, StatComponent>(registry))
	{
		

		skeletonComponent = registry.GetComponent<SkeletonBehaviour>(enemyEntity);
		skeletonTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

		if (skeletonComponent != nullptr && playerTransformCompenent!= nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			ML_Vector<Node> finalPath;
			skeletonComponent->updatePathCounter += GetDeltaTime();
			skeletonComponent->testUpdateTimer += GetDeltaTime();
			
			float distance = Calculate2dDistance(skeletonTransformComponent->positionX, skeletonTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			skeletonComponent->attackStunDurationCounter += GetDeltaTime();
			if (skeletonComponent->attackStunDurationCounter <= skeletonComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
				//Elliot: When finished, reset attack timer and hitbox
				skeletonComponent->attackTimer = 0.0f;
				enemyAnim->aAnimTime += (float)(enemyAnim->aAnimTime < 1.0f) * GetDeltaTime();
				continue;
			}
			else//Elliot: Turn off attack hitbox to not make player rage.
			{
				SetHitboxActive(enemyEntity, skeletonComponent->attackHitboxID, false);
				SetHitboxCanDealDamage(enemyEntity, skeletonComponent->attackHitboxID, false);
			}


			//Elliot: If in attack, keep attacking even if player is outside
			if (distance < 2.5f || skeletonComponent->attackTimer > 0.0f)
			{
				CombatBehaviour(skeletonComponent, enemyStats, playerStats, playerTransformCompenent, skeletonTransformComponent, enemyEntity, enemyAnim);
			}
			else if (distance < 590.f) //hunting distance
			{
				TransformComponent* doggoT;
				for (auto enemyEntity : View<HellhoundBehaviour, TransformComponent, StatComponent>(registry))
				{
					doggoT = registry.GetComponent<TransformComponent>(enemyEntity);
				}
				if (skeletonComponent->updatePathCounter >= skeletonComponent->updatePathLimit)
				{
					
					skeletonComponent->counterForTest = 0;
					skeletonComponent->updatePathCounter = 0;
					finalPath = CalculateAStarPath(playerComponent->mapID, valueGrid, skeletonTransformComponent, playerTransformCompenent);
					skeletonComponent->coolVec.clear();

					if (finalPath.size() > 1)
					{
						skeletonComponent->fx = finalPath[0].fx;
						skeletonComponent->fz = finalPath[0].fz;
					}
					

					for (int p = 0; p < finalPath.size(); p++)
					{
						finalPath[p].fx = skeletonComponent->fx;
						finalPath[p].fz = skeletonComponent->fz;
						skeletonComponent->coolVec.push_back((float)finalPath[p].x);
						skeletonComponent->coolVec.push_back((float)finalPath[p].z);
					}
					// goal (next node) - current
					if (finalPath.size() > 1)
					{
						skeletonComponent->dirX = finalPath[1].x - finalPath[0].x;
						skeletonComponent->dirZ = -(finalPath[1].z - finalPath[0].z);
						skeletonComponent->dir2X = finalPath[2].x - finalPath[1].x;
						skeletonComponent->dir2Z = -(finalPath[2].z - finalPath[1].z);
						skeletonComponent->followPath = true;
					}
					else
					{
 						skeletonComponent->followPath = false;
					}
				}   
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

					gridOnPos = GridOnPosition(coorde, ggg);
					doggoT->positionX = gridOnPos.x;
					doggoT->positionZ = gridOnPos.z;


					skeletonComponent->counterForTest += 2;
				}
				if (distance < 1.f)
				{
					skeletonComponent->followPath = false;
				}
				for (auto enemyEntity : View<HellhoundBehaviour, TransformComponent, StatComponent>(registry))
				{


					/*HellhoundBehaviour*doggoB = registry.GetComponent<HellhoundBehaviour>(enemyEntity);
					TransformComponent* doggoT = registry.GetComponent<TransformComponent>(enemyEntity);
					
					
					doggoT->facingX = skeletonComponent->dirX;
					doggoT->facingZ = skeletonComponent->dirZ;*/
				
				}
				if (skeletonComponent->followPath == true && skeletonComponent->updatePathCounter >= skeletonComponent->updatePathLimit / 2.f  )
				{
					skeletonComponent->dirX = skeletonComponent->dir2X;
					skeletonComponent->dirZ = skeletonComponent->dir2Z;
				}
				
				ChaseBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim, skeletonComponent->dirX, skeletonComponent->dirZ, skeletonComponent->followPath);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim);
			}
		}
	}


	return true;
}