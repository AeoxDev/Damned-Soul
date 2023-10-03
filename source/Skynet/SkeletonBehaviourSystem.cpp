#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include <random>



void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent)
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


	//speed set to 10.0f, use enemy component later
	skeletonTransformComponent->positionX += dirX * 10.0f * GetDeltaTime();
	skeletonTransformComponent->positionZ += dirZ * 10.0f * GetDeltaTime();
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent)
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

	
	//speed set to 10.0f, use enemy component later
	skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * 5.0f * GetDeltaTime();
	skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * 5.0f * GetDeltaTime();
	
}

void CombatBehaviour(SkeletonBehaviour* skeletonComponent)
{

}

bool SkeletonBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	SkeletonBehaviour* skeletonComponent = nullptr;
	TransformComponent* skeletonTransformComponent = nullptr;
	
	for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
	}

	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent>(registry))
	{
		skeletonComponent = registry.GetComponent<SkeletonBehaviour>(enemyEntity);
		skeletonTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);

		if (skeletonComponent != nullptr && playerTransformCompenent!= nullptr && true )// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(skeletonTransformComponent->positionX, skeletonTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			if (distance < 2.5f)
			{
				CombatBehaviour(skeletonComponent);
			}
			else if (distance < 50) //hunting distance
			{
				ChaseBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent);
			}
		}
	}


	return true;
}