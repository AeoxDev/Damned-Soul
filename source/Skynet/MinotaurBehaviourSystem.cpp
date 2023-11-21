#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Skynet\BehaviourHelper.h"
#include "EventFunctions.h"

#include <random>

bool MinotaurBehaviourSystem::Update()
{
	////First find the skynet component
	//PlayerComponent* playerComponent = nullptr;
	//TransformComponent* playerTransformCompenent = nullptr;
	//MinotaurBehaviour* minoComp = nullptr;
	//TransformComponent* minoTransformComponent = nullptr;
	//StatComponent* enemyStats = nullptr;
	//StatComponent* playerStats = nullptr;
	//EnemyComponent* enmComp = nullptr;
	//AnimationComponent* enemyAnim = nullptr;

	//bool updateGridOnce = true;

	//PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));


	//for (auto enemyEntity : View<MinotaurBehaviour, TransformComponent, StatComponent, EnemyComponent, AnimationComponent>(registry))
	//{
	//	minoComp = registry.GetComponent<MinotaurBehaviour>(enemyEntity);
	//	minoTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
	//	enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
	//	enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);
	//	enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

	//	//Find a player to kill.
	//	if (enmComp != nullptr)
	//	{
	//		if (enmComp->lastPlayer.index == -1)
	//		{
	//			for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	//			{
	//				if (enemyEntity.index == playerEntity.index)
	//				{
	//					continue;
	//				}
	//				playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
	//				playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
	//				playerStats = registry.GetComponent< StatComponent>(playerEntity);
	//				enmComp->lastPlayer = playerEntity;
	//				if (rand() % 2)
	//				{
	//					break;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			playerComponent = registry.GetComponent<PlayerComponent>(enmComp->lastPlayer);
	//			playerTransformCompenent = registry.GetComponent<TransformComponent>(enmComp->lastPlayer);
	//			playerStats = registry.GetComponent< StatComponent>(enmComp->lastPlayer);
	//			if (playerComponent == nullptr)
	//			{
	//				for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	//				{
	//					enmComp->lastPlayer.index = -1;
	//				}
	//			}
	//		}
	//	}

	//	//ACTUAL BEHAVIOUR
	//	if (minoTransformComponent != nullptr && playerTransformCompenent != nullptr && enemyAnim != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
	//	{
	//		float distance = Calculate2dDistance(minoTransformComponent->positionX, minoTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);

	//		if (distance < 10.f)
	//		{

	//		}
	//		else if (minoComp->jumping) // this is the fly up in air thing. jump jump
	//		{
	//			minoComp->airTimer += GetDeltaTime();
	//			if (minoComp->airTimer <= minoComp->airDuration) // fly up in the air
	//			{
	//				minoTransformComponent->positionY += enemyStats->GetSpeed() * 7.f * GetDeltaTime();
	//			}
	//			else  // fly down from air
	//			{
	//				if (playerComponent != nullptr && updateGridOnce && minoComp->hasLandingPos == false)
	//				{
	//					updateGridOnce = false;
	//					CalculateGlobalMapValuesLuciferJump(valueGrid); //generate a gridmap
	//					if (valueGrid->cost[0][0] == -69.f)
	//					{
	//						updateGridOnce = true; //illegal grid
	//						continue;
	//					}
	//					TransformComponent landingPosition = FindRetreatTile(valueGrid, playerTransformCompenent, 10.f, 20.f); // where to land
	//					minoComp->hasLandingPos = true;
	//					minoTransformComponent->positionX = landingPosition.positionX; //teleport in the air basically
	//					minoTransformComponent->positionZ = landingPosition.positionZ; // happens once
	//				}
	//				if (minoTransformComponent->positionY > 0.f) // still in the air
	//				{
	//					minoTransformComponent->positionY -= enemyStats->GetSpeed() * 12.f * GetDeltaTime();
	//				}
	//				else // on ground or below ground, set to ground and SHOCKWAVE with timed event
	//				{
	//					//reset variables to beheaviour. No more jump, get dazed
	//					minoComp->jumping = false;
	//					//minoComp->isDazed = true;
	//					SetHitboxActive(enemyEntity, 0, true);
	//					SetHitboxActive(enemyEntity, 1, true);
	//					SetHitboxActive(enemyEntity, 2, true);
	//					minoTransformComponent->positionY = 0.f;

	//					//shockwave here
	//					AddTimedEventComponentStartContinuousEnd(enemyEntity, 0.0f, BossShockwaveStart, BossShockwaveExpand, 4.0f, BossShockwaveEnd, 0, 1);
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//free(valueGrid);
	return true;
}