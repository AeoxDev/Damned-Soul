#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include "UI/UIRenderer.h"
#include <random>


void RepositionBehaviour(ImpBehaviour* ic, TransformComponent* itc, TransformComponent* ptc, PathfindingMap* valueGrid)
{
	//Reset teleport counter
	ic->specialCounter = 0;

	//Calculate new teleport breakpoint
	std::random_device rd;
	std::mt19937 gen(rd());
	// Define a uniform distribution for the range [1.0, 5.0]
	std::uniform_real_distribution<float> distribution(1.0f, 5.0f);
	ic->specialBreakpoint = (int)distribution(gen);

	//Teleport
	float minRange = 20.0f;
	float maxRange = 40.0f;

	TransformComponent newTransform = FindRetreatTile(valueGrid, ptc, minRange, maxRange);
	itc->positionX = newTransform.positionX;
	itc->positionZ = newTransform.positionZ;
}

void RetreatBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, ImpBehaviour* ic, TransformComponent* itc, StatComponent* enemyStats, AnimationComponent* enemyAnim, PathfindingMap* valueGrid, bool& hasUpdatedMap)
{
	// Regular walk
	enemyAnim->aAnim = ANIMATION_WALK;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime();
	ANIM_BRANCHLESS(enemyAnim);

	ic->chaseCounter += GetDeltaTime();
	//if the player has chased the imp for too long, teleport away
	if (ic->chaseCounter >= ic->chaseTimer)
	{
		ic->chaseCounter = 0.0f;
		
		if (hasUpdatedMap == false)
		{
			hasUpdatedMap = true;
			CalculateGlobalMapValuesHellhound(valueGrid);
		}

		RepositionBehaviour(ic, itc, playerTransformCompenent, valueGrid);
	}
	else
	{
		//calculate the direction away from the player
		ic->goalDirectionX = -(playerTransformCompenent->positionX - itc->positionX);
		ic->goalDirectionZ = -(playerTransformCompenent->positionZ - itc->positionZ);
		float magnitude = sqrt(ic->goalDirectionX * ic->goalDirectionX + ic->goalDirectionZ * ic->goalDirectionZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		ic->goalDirectionX /= magnitude;
		ic->goalDirectionZ /= magnitude;
		SmoothRotation(itc, ic->goalDirectionX, ic->goalDirectionZ, 30.f);
		float dirX = itc->facingX, dirZ = itc->facingZ;
		magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}

		itc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
		itc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
	}
}

bool CombatBehaviour(EntityID entity, PlayerComponent*& pc, TransformComponent*& ptc, ImpBehaviour*& ic, TransformComponent*& itc, StatComponent*& enemyStats, StatComponent*& playerStats, AnimationComponent* enemyAnim)
{
	//if you just attacked go back to circle behaviour
	if (ic->attackTimer < enemyStats->GetAttackSpeed())
	{
		ic->attackTimer += GetDeltaTime();
		return false;
	}
	//rotate imp in order to shoot at the player
	else if (ic->aimTimer < ic->aimDuration)
	{
		ic->goalDirectionX = ptc->positionX - itc->positionX;
		ic->goalDirectionZ = ptc->positionZ - itc->positionZ;
		float magnitude = sqrt(ic->goalDirectionX * ic->goalDirectionX + ic->goalDirectionZ * ic->goalDirectionZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		ic->goalDirectionX /= magnitude;
		ic->goalDirectionZ /= magnitude;


		SmoothRotation(itc, ic->goalDirectionX, ic->goalDirectionZ, 30.f);
		ic->aimTimer += GetDeltaTime();
		ic->attackTimer += GetDeltaTime();
		return true;
	}
	else // yes, we can indeed attack. 
	{
		enemyAnim->aAnim = ANIMATION_ATTACK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
		ANIM_BRANCHLESS(enemyAnim);

		ic->attackTimer = 0;
		ic->attackStunDurationCounter = 0;
		ic->aimTimer = 0;
		ic->specialCounter++; //increase the special counter for special attack

		//set direction for attack
		float dx = (ptc->positionX - itc->positionX);
		float dz = (ptc->positionZ - itc->positionZ);

		//normalize initial direction
		float magnitude = sqrt(dx * dx + dz * dz);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}

		dx /= magnitude;
		dz /= magnitude;

		//		C		= sqrt(A^2 + B^2)
		float newDirX = sqrt(ptc->facingX * ptc->facingX + dx * dx);
		float newDirZ = sqrt(ptc->facingZ * ptc->facingZ + dz * dz);

		//how much the player moved since last frame
		float playerMovementX = ptc->positionX - ptc->lastPositionX;
		float playerMovementZ = ptc->positionZ - ptc->lastPositionZ;

		//limit player movement in case of dash
		if (playerMovementX > 0.02f)
			playerMovementX = 0.02f;
		if (playerMovementZ > 0.02f)
			playerMovementZ = 0.02f;

		//calculate final direction based on how much the player moved plus the player movespeed
		dx += newDirX * playerMovementX * playerStats->GetSpeed();
		dz += newDirZ * playerMovementZ * playerStats->GetSpeed();

		//normalize finial direction
		magnitude = sqrt(dx * dx + dz * dz);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		dx /= magnitude;
		dz /= magnitude;

		ic->goalDirectionX = dx;
		ic->goalDirectionZ = dz;

		SmoothRotation(itc, ic->goalDirectionX, ic->goalDirectionZ, 30.f);
		CreateProjectile(entity, dx, dz, imp);
		return true;
	}
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, ImpBehaviour* ic, TransformComponent* itc, StatComponent* enemyStats, AnimationComponent* enemyAnim, PathfindingMap* valueGrid, bool& hasUpdatedMap)
{
	//idle just do animation
	enemyAnim->aAnim = ANIMATION_IDLE;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime();
	ANIM_BRANCHLESS(enemyAnim);

	ic->idleCounter += GetDeltaTime();
	if (ic->idleCounter >= ic->idleTimer)
	{
		ic->idleCounter = 0.0f;

		if (hasUpdatedMap == false)
		{
			hasUpdatedMap = true;
			CalculateGlobalMapValuesHellhound(valueGrid);
		}

		RepositionBehaviour(ic, itc, itc, valueGrid);
	}
}

bool ImpBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	ImpBehaviour* impComponent = nullptr;
	TransformComponent* impTransformComponent = nullptr;
	HitboxComponent* enemyHitbox = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enemComp = nullptr;

	bool hasUpdatedMap = false;
	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));


	//Find available entity
	for (auto enemyEntity : View<ImpBehaviour, TransformComponent, HitboxComponent, EnemyComponent>(registry))
	{

		SetLightColor(enemyEntity, 0.3f, 0.3f, 0.3f);
		impComponent = registry.GetComponent<ImpBehaviour>(enemyEntity);
		impTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		enemyHitbox = registry.GetComponent<HitboxComponent>(enemyEntity);
		enemComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		AnimationComponent* enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);
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


		if (enemyStats->GetHealth() > 0 && impComponent != nullptr && playerTransformCompenent != nullptr && enemyHitbox != nullptr && enemComp != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(impTransformComponent->positionX, impTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);

			impComponent->attackStunDurationCounter += GetDeltaTime();
			if (impComponent->attackStunDurationCounter <= impComponent->attackStunDuration)
			{
				// Regular attack?
				enemyAnim->aAnim = ANIMATION_IDLE;
				enemyAnim->aAnimIdx = 0;
				enemyAnim->aAnimTime += GetDeltaTime() * .7f;
				ANIM_BRANCHLESS(enemyAnim);
			}
			else if (distance < 15.0f && !impComponent->charging) // try to retreat to a safe distance if not charging
			{
				RetreatBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim, valueGrid, hasUpdatedMap);
			}
			else if (impComponent->specialCounter >= impComponent->specialBreakpoint) //if special is ready teleport
			{
				if (hasUpdatedMap == false)
				{
					hasUpdatedMap = true;
					CalculateGlobalMapValuesImp(valueGrid);
				}

				RepositionBehaviour(impComponent, impTransformComponent, playerTransformCompenent, valueGrid);
			}
			else if (distance <= 50.0f + impComponent->circleBehaviour) // circle player & attack when possible (WIP)
			{
				if (!CombatBehaviour(enemyEntity, playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, playerStats, enemyAnim))
					IdleBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim, valueGrid, hasUpdatedMap);
			}
			else // idle
			{
				enemComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim, valueGrid, hasUpdatedMap);
			}
		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enemComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim, valueGrid, hasUpdatedMap);
		}
		TransformDecelerate(enemyEntity);
	}
	
	free(valueGrid);

	return true;
}