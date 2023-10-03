#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include <random>
#include "Skynet\BehaviourHelper.h"




void CombatBehaviour(HellhoundBehaviour* hellhoundComponent)
{

}


void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, HellhoundBehaviour* hellhoundComponent, TransformComponent*  hellhoundTransformComponent)
{

}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, HellhoundBehaviour* hellhoundComponent, TransformComponent* hellhoundTransformComponent)
{
	hellhoundComponent->timeCounter += GetDeltaTime();
	if (hellhoundComponent->timeCounter >= hellhoundComponent->updateInterval)
	{
		hellhoundComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		hellhoundComponent->goalDirectionX = randomX;
		hellhoundComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		hellhoundComponent->updateInterval = randomInterval(gen);
	}

	SmoothRotation(hellhoundTransformComponent, hellhoundComponent->goalDirectionX, hellhoundComponent->goalDirectionZ);


	//speed set to 10.0f, use enemy component later
	hellhoundTransformComponent->positionX += hellhoundTransformComponent->facingX * 5.0f * GetDeltaTime();
	hellhoundTransformComponent->positionZ += hellhoundTransformComponent->facingZ * 5.0f * GetDeltaTime();
}










bool HellhonudBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	HellhoundBehaviour* hellhoundComponent = nullptr;
	TransformComponent* hellhoundTransformComponent = nullptr;
	

	for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
	}

	for (auto enemyEntity : View<HellhoundBehaviour, TransformComponent>(registry))
	{
		hellhoundComponent = registry.GetComponent<HellhoundBehaviour>(enemyEntity);
		hellhoundTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);

		if (hellhoundComponent != nullptr && playerTransformCompenent != nullptr && true)// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(hellhoundTransformComponent->positionX, hellhoundTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);

			if (distance < 2.5f)
			{
				CombatBehaviour(hellhoundComponent);
			}
			else if (distance < 60) //hunting distance
			{
				ChaseBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
		}
	}
	return true;
}