#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include <random>


void CombatBehaviour(EyeBehaviour* eyeComponent, StatComponent* enemyStats, StatComponent* playerStats)
{
	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (eyeComponent->attackTimer >= enemyStats->attackSpeed) // yes, we can indeed attack. 
	{
		eyeComponent->attackTimer = 0;
		eyeComponent->attackStunDurationCounter = 0;
		playerStats->health -= enemyStats->damage;
	}
}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, EyeBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats)
{
	float relativePosX = ptc->positionX - etc->positionX;
	float relativePosZ = ptc->positionZ - etc->positionZ;

	float relativeDirectionX = ptc->facingX - etc->facingX;
	float relativeDirectionZ = ptc->facingZ - etc->facingZ;

	//this will be used to determine if we are exactly
	 // a = spelare. b = hellhound
	float playerToHellhoundX = etc->positionX - ptc->positionX;
	float playerToHellhoundZ = etc->positionZ - ptc->positionZ;
	float behindDot = playerToHellhoundX * ptc->facingX + playerToHellhoundZ * ptc->facingZ;
	float magHellhound = sqrt(playerToHellhoundX * playerToHellhoundX + playerToHellhoundZ * playerToHellhoundZ);
	float magPlayer = sqrt(ptc->facingX * ptc->facingX + ptc->facingZ * ptc->facingZ);

	float tolerance = 0.3; // THIS IS FOR ANGLE SMOOTHING

	float dot = relativePosX * relativeDirectionZ - relativePosZ * relativeDirectionX;

	float magnitude = 0.f;
	float dirX = 0.f;
	float dirZ = 0.f;
	if (!ec->circleBehaviour)
	{
		ec->circleBehaviour = true;
		if (dot < 0) // clockwise
		{
			ec->clockwiseCircle = true;
		}
		else // counter clockwise
		{
			ec->clockwiseCircle = false;
		}
	}

	if (ec->clockwiseCircle) //clockwise
	{
		dirX = -ec->goalDirectionZ;
		dirZ = ec->goalDirectionX;
		magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		SmoothRotation(etc, dirX, dirZ);
	}
	else // counter clockwise
	{
		dirX = ec->goalDirectionZ;
		dirZ = -ec->goalDirectionX;
		magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		SmoothRotation(etc, dirX, dirZ);
	}
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}
	etc->positionX += dirX * enemyStats->moveSpeed * GetDeltaTime();
	etc->positionZ += dirZ * enemyStats->moveSpeed * GetDeltaTime();
	ec->goalDirectionX = ptc->positionX - etc->positionX;
	ec->goalDirectionZ = ptc->positionZ - etc->positionZ;
}


void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats)
{
	eyeComponent->goalDirectionX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
	eyeComponent->goalDirectionZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);
	float dirX = eyeTransformComponent->facingX, dirZ = eyeTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	//speed set to 10.0f, use enemy component later
	float speedMultiplier = 1.f;
	if (eyeComponent->charge)
	{
		speedMultiplier = 2.f;
	}
	eyeTransformComponent->positionX += dirX * enemyStats->moveSpeed * speedMultiplier * GetDeltaTime();
	eyeTransformComponent->positionZ += dirZ * enemyStats->moveSpeed * speedMultiplier * GetDeltaTime();
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats)
{
	eyeComponent->timeCounter += GetDeltaTime();
	if (eyeComponent->timeCounter >= eyeComponent->updateInterval)
	{
		eyeComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		eyeComponent->goalDirectionX = randomX;
		eyeComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		eyeComponent->updateInterval = randomInterval(gen);
	}

	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);

	eyeTransformComponent->positionX += eyeTransformComponent->facingX * enemyStats->moveSpeed / 2.f * GetDeltaTime();
	eyeTransformComponent->positionZ += eyeTransformComponent->facingZ * enemyStats->moveSpeed / 2.f * GetDeltaTime();
}


bool EyeBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	EyeBehaviour* eyeComponent = nullptr;
	TransformComponent* eyeTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;

	for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
		playerStats = registry.GetComponent< StatComponent>(playerEntity);
	}

	for (auto enemyEntity : View<EyeBehaviour, TransformComponent>(registry))
	{
		eyeComponent = registry.GetComponent<EyeBehaviour>(enemyEntity);
		eyeTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent<StatComponent>(enemyEntity);

		if (enemyStats->health > 0 && eyeComponent != nullptr && playerTransformCompenent != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(eyeTransformComponent->positionX, eyeTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			eyeComponent->attackTimer += GetDeltaTime();
			eyeComponent->attackStunDurationCounter += GetDeltaTime();
			if (eyeComponent->attackStunDurationCounter <= eyeComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
			}
			else if (distance < 2.5f) // fight club
			{
				CombatBehaviour(eyeComponent, enemyStats, playerStats);
			}
			else if (distance <= 15 + eyeComponent->circleBehaviour) // circle player
			{
				CircleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
			}
			else if (distance < 50) //hunting distance, go chase
			{
				ChaseBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
			}
		}
		else
		{
			//COMMIT DIE
		}
	}
	return true;
}