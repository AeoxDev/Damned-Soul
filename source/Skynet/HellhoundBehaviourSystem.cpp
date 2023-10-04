#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include <random>
#include "Skynet\BehaviourHelper.h"



// input true on stuff you want to reset
void ResetHellhoundVariables(HellhoundBehaviour* hc, bool circleBehavior, bool charge)
{
	if (circleBehavior)
	{
		hc->giveUpChaseCounter += GetDeltaTime();
		if (hc->giveUpChaseCounter >= 1.0f)
		{
			hc->isBehind = false;
			hc->isBehindCounter = 0.f;
			hc->circleBehaviour = false;
			hc->giveUpChaseCounter = 0.f;
		}
	}
	if (charge)
	{
		hc->charge = false;
	}
	
}

void CombatBehaviour(HellhoundBehaviour* hellhoundComponent)
{

}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, HellhoundBehaviour* hc, TransformComponent* htc)
{
	float relativePosX = ptc->positionX - htc->positionX;
	float relativePosZ = ptc->positionZ - htc->positionZ;

	float relativeDirectionX = ptc->facingX - htc->facingX;
	float relativeDirectionZ = ptc->facingZ - htc->facingZ;

	//this will be used to determine if we are exactly
	 // a = spelare. b = hellhound
	float playerToHellhoundX = htc->positionX - ptc->positionX;
	float playerToHellhoundZ = htc->positionZ - ptc->positionZ;
	float behindDot = playerToHellhoundX * ptc->facingX + playerToHellhoundZ * ptc->facingZ;
	float magHellhound = sqrt(playerToHellhoundX * playerToHellhoundX + playerToHellhoundZ * playerToHellhoundZ);
	float magPlayer = sqrt(ptc->facingX * ptc->facingX + ptc->facingZ * ptc->facingZ);

	float tolerance = 0.2; // THIS IS FOR ANGLE SMOOTHING
	if (std::abs((behindDot / (magHellhound * magPlayer) + 1)) < tolerance) // are we behind player back? (trust the magic math, please)
	{
		hc->isBehind = true;
		hc->isBehindCounter += GetDeltaTime();
	}
	else
	{
		ResetHellhoundVariables(hc, true, true);
	}


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
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}
	htc->positionX += dirX * 10.0f * GetDeltaTime();
	htc->positionZ += dirZ * 10.0f * GetDeltaTime();
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
	float speed = 10.f;
	if (hellhoundComponent->charge)
	{
		speed = 18.f;
	}
	hellhoundTransformComponent->positionX += dirX * speed * GetDeltaTime();
	hellhoundTransformComponent->positionZ += dirZ * speed * GetDeltaTime();
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

			if (distance < 3.5f) // fight club
			{
				ResetHellhoundVariables(hellhoundComponent, true, true);
				CombatBehaviour(hellhoundComponent);
			}
			else if (distance <= 15 + hellhoundComponent->circleBehaviour) // circle player
			{
				if (hellhoundComponent->isBehind && hellhoundComponent->isBehindCounter >= 0.3f) // attack the back
				{
					hellhoundComponent->charge = true;
					ChaseBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
				}
				else //keep circling
				{
					CircleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
				}
			}
			else if (distance < 50) //hunting distance, go chase
			{
				ResetHellhoundVariables(hellhoundComponent, true, true);
				ChaseBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
			else // idle
			{
				ResetHellhoundVariables(hellhoundComponent, true, true);
				IdleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent);
			}
		}
	}
	return true;
}