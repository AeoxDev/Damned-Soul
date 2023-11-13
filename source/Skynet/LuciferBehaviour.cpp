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
			//hellhoundComponent->updatePathCounter += GetDeltaTime();
			luciferComponent->spawnCounter += GetDeltaTime();

			float distance = Calculate2dDistance(luciferTransformComponent->positionX, luciferTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			//hellhoundComponent->attackStunDurationCounter += GetDeltaTime();

			if (luciferComponent->spawnCounter >= luciferComponent->spawnTiming)
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
				for (int i = 0; i < levelOfDamage; i++)
				{
					int xPos = rand() % 60 - 30;
					int zPos = rand() % 60 - 30;
					SetupEnemy(EnemyType::frozen1, (float)xPos, 0.f, (float)zPos);
				}
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