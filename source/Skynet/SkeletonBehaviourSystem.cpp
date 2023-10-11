#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>



void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent, StatComponent* stats)
{
	skeletonComponent->goalDirectionX = playerTransformCompenent->positionX - skeletonTransformComponent->positionX;
	skeletonComponent->goalDirectionZ = playerTransformCompenent->positionZ - skeletonTransformComponent->positionZ;
	
	SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ);
	float dirX = skeletonTransformComponent->facingX, dirZ = skeletonTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	skeletonTransformComponent->positionX += dirX * stats->moveSpeed * GetDeltaTime();
	skeletonTransformComponent->positionZ += dirZ * stats->moveSpeed * GetDeltaTime();
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent, StatComponent* stats)
{
	skeletonComponent->timeCounter += GetDeltaTime();
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

	
	skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * stats->moveSpeed/ 2.f * GetDeltaTime();
	skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * stats->moveSpeed / 2.f * GetDeltaTime();
	
}

void CombatBehaviour(SkeletonBehaviour* sc, StatComponent* enemyStats, StatComponent* playerStats)
{
	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (sc->attackTimer >= enemyStats->attackSpeed) // yes, we can indeed attack. 
	{
		sc->attackTimer = 0;
		sc->attackStunDurationCounter = 0;
		playerStats->health -= enemyStats->damage;
		RedrawUI();
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
	
	for (auto playerEntity : View<PlayerComponent, TransformComponent, StatComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
		playerStats = registry.GetComponent< StatComponent>(playerEntity);
	}

	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent, StatComponent>(registry))
	{
		skeletonComponent = registry.GetComponent<SkeletonBehaviour>(enemyEntity);
		skeletonTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);

		if (skeletonComponent != nullptr && playerTransformCompenent!= nullptr && enemyStats->health > 0 )// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(skeletonTransformComponent->positionX, skeletonTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			skeletonComponent->attackTimer += GetDeltaTime();
			skeletonComponent->attackStunDurationCounter += GetDeltaTime();

			if (skeletonComponent->attackStunDurationCounter <= skeletonComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
			}
			else if (distance < 2.5f)
			{
				CombatBehaviour(skeletonComponent, enemyStats, playerStats);
			}
			else if (distance < 50) //hunting distance
			{
				ChaseBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats);
			}
		}
	}


	return true;
}