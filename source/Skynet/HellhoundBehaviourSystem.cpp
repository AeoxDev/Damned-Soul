#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include <random>
#include "Skynet\BehaviourHelper.h"




void CombatBehaviour(HellhoundBehaviour* hellhoundComponent)
{

}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, HellhoundBehaviour* hc, TransformComponent* htc)
{
	float relativePosX = ptc->positionX - htc->positionX;
	float relativePosZ = ptc->positionZ - htc->positionZ;

	float relativeDirectionX = ptc->facingX - htc->facingX;
	float relativeDirectionZ = ptc->facingZ - htc->facingZ;

	float dot = relativePosX * relativeDirectionZ - relativePosZ * relativeDirectionX;
	float magnitude = 0.f;
	float dirX = 0.f;
	float dirZ = 0.f;
	if (!hc->circleBehaviour)
	{
		hc->circleBehaviour = true;
		if (dot < 0) // clockwise
		{
			hc->clockwiseCircle = true;
		}
		else // counter clockwise
		{
			hc->clockwiseCircle = false;
		}
	}

	if (hc->clockwiseCircle) //clockwise
	{
		dirX = -hc->goalDirectionZ;
		dirZ = hc->goalDirectionX;
		magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		SmoothRotation(htc, dirX, dirZ);
	}
	else // counter clockwise
	{
		dirX = hc->goalDirectionZ;
		dirZ = -hc->goalDirectionX;
		magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		SmoothRotation(htc, dirX, dirZ);
	}

	htc->positionX += dirX/ magnitude * 10.0f * GetDeltaTime();
	htc->positionZ += dirZ / magnitude * 10.0f * GetDeltaTime();
	hc->goalDirectionX = ptc->positionX - htc->positionX;
	hc->goalDirectionZ = ptc->positionZ - htc->positionZ;
}


void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, HellhoundBehaviour* hellhoundComponent, TransformComponent*  hellhoundTransformComponent)
{
	hellhoundComponent->goalDirectionX = playerTransformCompenent->positionX - hellhoundTransformComponent->positionX;
	hellhoundComponent->goalDirectionZ = playerTransformCompenent->positionZ - hellhoundTransformComponent->positionZ;

	SmoothRotation(hellhoundTransformComponent, hellhoundComponent->goalDirectionX, hellhoundComponent->goalDirectionZ);
	float dirX = hellhoundTransformComponent->facingX, dirZ = hellhoundTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}


	//speed set to 10.0f, use enemy component later
	hellhoundTransformComponent->positionX += dirX * 10.0f * GetDeltaTime();
	hellhoundTransformComponent->positionZ += dirZ * 10.0f * GetDeltaTime();
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

			if (distance < 3.5f)
			{
				hellhoundComponent->circleBehaviour = false;
				CombatBehaviour(hellhoundComponent);
			}
			else if (distance <= 12 + hellhoundComponent->circleBehaviour)
			{
				CircleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
			else if (distance < 50) //hunting distance
			{
				hellhoundComponent->circleBehaviour = false;
				ChaseBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
			else // idle
			{
				hellhoundComponent->circleBehaviour = false;
				IdleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
		}
	}
	return true;
}