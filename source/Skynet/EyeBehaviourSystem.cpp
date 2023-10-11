#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include <random>


void RetreatBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats)
{
	eyeComponent->goalDirectionX = -(playerTransformCompenent->positionX - eyeTransformComponent->positionX);
	eyeComponent->goalDirectionZ = -(playerTransformCompenent->positionZ - eyeTransformComponent->positionZ);

	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);
	float dirX = eyeTransformComponent->facingX, dirZ = eyeTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	eyeTransformComponent->positionX += dirX * enemyStats->moveSpeed * GetDeltaTime();
	eyeTransformComponent->positionZ += dirZ * enemyStats->moveSpeed * GetDeltaTime();

}

bool CombatBehaviour(PlayerComponent*& pc, TransformComponent*& ptc, EyeBehaviour*& ec, TransformComponent*& etc, StatComponent*& enemyStats, StatComponent*& playerStats)
{
	//impose timer so they cannot run and hit at the same time also not do a million damage per sec
	if (ec->attackTimer >= enemyStats->attackSpeed) // yes, we can indeed attack. 
	{
		ec->attackTimer = 0;
		ec->attackStunDurationCounter = 0;
		ec->specialCounter++; //increase the special counter for special attack

		//set direction for attack
		float dx = ptc->positionX - etc->positionX;
		float dz = ptc->positionZ - etc->positionZ;
		float magnitude = sqrt(dx * dx + dz * dz);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}

		float orthoX = -dz;
		float orthoZ = dx;

		dx /= magnitude;
		dz /= magnitude;

		float targetX = etc->positionX + dx * 10.0f;
		float targetZ = etc->positionZ + dz * 10.0f;

		ec->facingX = targetX;
		ec->facingZ = targetZ;

		//SmoothRotation(etc, ec->facingX, ec->facingZ);
		

		//SHOOOT

		//playerStats->health -= enemyStats->damage;
		//RedrawUI();

		return true;
	}
	else
	{
		return false;
	}
}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, EyeBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats, StatComponent* playerStats)
{
	float relativePosX = ptc->positionX - etc->positionX;
	float relativePosZ = ptc->positionZ - etc->positionZ;

	float relativeDirectionX = ptc->facingX - etc->facingX;
	float relativeDirectionZ = ptc->facingZ - etc->facingZ;

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
	}
	else // counter clockwise
	{
		dirX = ec->goalDirectionZ;
		dirZ = -ec->goalDirectionX;
	}
	magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	SmoothRotation(etc, dirX, dirZ);
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

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats)
{
	eyeComponent->timeCounter += GetDeltaTime();
	if (eyeComponent->timeCounter >= eyeComponent->updateInterval)
	{
		eyeComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distributionX(eyeTransformComponent->facingX - 0.3f, eyeTransformComponent->facingX + 0.3f);
		std::uniform_real_distribution<float> distributionZ(eyeTransformComponent->facingZ - 0.3f, eyeTransformComponent->facingZ + 0.3f);
		float randomX = distributionX(gen);
		float randomZ = distributionZ(gen);
		eyeComponent->goalDirectionX = randomX;
		eyeComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.4f, 0.8f);
		eyeComponent->updateInterval = randomInterval(gen);
	}

	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);

	eyeTransformComponent->positionX += eyeTransformComponent->facingX * enemyStats->moveSpeed / 2.f * GetDeltaTime();
	eyeTransformComponent->positionZ += eyeTransformComponent->facingZ * enemyStats->moveSpeed / 2.f * GetDeltaTime();
}

void ChargeBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats, HitboxComponent* enemyHitbox, EntityID eID)
{
	if (!eyeComponent->charging)
	{
		eyeComponent->specialCounter = 0;
		eyeComponent->charging = true;

		SetHitboxIsMoveable(eID, 0, false);
		SetHitboxIsMoveable(eID, 1, false);

		float dirX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
		float dirZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

		float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}

		dirX /= magnitude;
		dirZ /= magnitude;

		eyeComponent->targetX = playerTransformCompenent->positionX + dirX * 5.0f;
		eyeComponent->targetZ = playerTransformCompenent->positionZ + dirZ * 5.0f;

		eyeComponent->goalDirectionX = dirX;
		eyeComponent->goalDirectionZ = dirZ;
		eyeComponent->changeDirX = dirX;
		eyeComponent->changeDirZ = dirZ;

		SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);

	}
	else 
	{
		//First enemy to point direction
		//Then charging direction
		//If charging scalar point direction > 0.0, charge

		float dirX = (eyeComponent->targetX - eyeTransformComponent->positionX);
		float dirZ = (eyeComponent->targetZ - eyeTransformComponent->positionZ);

		float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		dirX /= magnitude;
		dirZ /= magnitude;

		float scalar = dirX * eyeComponent->changeDirX + dirZ * eyeComponent->changeDirZ;

		if (scalar < 0)
		{
			eyeComponent->charging = false;

			SetHitboxIsMoveable(eID, 0, true);
			SetHitboxIsMoveable(eID, 1, true);
		}
		else
		{
			SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);

			eyeTransformComponent->positionX += eyeTransformComponent->facingX * enemyStats->moveSpeed * 4.f * GetDeltaTime();
			eyeTransformComponent->positionZ += eyeTransformComponent->facingZ * enemyStats->moveSpeed * 4.f * GetDeltaTime();
		}
	}
}

static int REMOVEMEPLEASE = 0;

bool EyeBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	EyeBehaviour* eyeComponent = nullptr;
	TransformComponent* eyeTransformComponent = nullptr;
	HitboxComponent* enemyHitbox = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;

	for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
		playerStats = registry.GetComponent< StatComponent>(playerEntity);
	}
	
	for (auto enemyEntity : View<EyeBehaviour, TransformComponent, HitboxComponent>(registry))
	{
		eyeComponent = registry.GetComponent<EyeBehaviour>(enemyEntity);
		eyeTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		enemyHitbox = registry.GetComponent<HitboxComponent>(enemyEntity);


		if (enemyStats->health > 0 && eyeComponent != nullptr && playerTransformCompenent != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(eyeTransformComponent->positionX, eyeTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			eyeComponent->attackTimer += GetDeltaTime();
			eyeComponent->attackStunDurationCounter += GetDeltaTime();
			if (eyeComponent->attackStunDurationCounter <= eyeComponent->attackStunDuration)
			{
				// do nothing, stand still and be ashamed
			}
			else if (distance < 15.0f && !eyeComponent->charging) // Retreat to safe distance
			{
				RetreatBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
			}
			else if (eyeComponent->specialCounter > eyeComponent->specialBreakpoint)
			{
				//CHAAAAARGE
				
				ChargeBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyHitbox, enemyEntity);

			}
			else if (distance <= 30.0f + eyeComponent->circleBehaviour) // circle player & attack when possible
			{
				//SmoothRotation(eyeTransformComponent, eyeComponent->facingX, eyeComponent->facingZ);
				//if(!CombatBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats))
				CircleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats);
				REMOVEMEPLEASE++;
				if (REMOVEMEPLEASE % 20 == 0)
				{
					eyeComponent->specialCounter++;
				}

			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
				REMOVEMEPLEASE++;
				if (REMOVEMEPLEASE % 20 == 0)
				{
					eyeComponent->specialCounter++;
				}
			}
		}
		else
		{
			//COMMIT DIE
		}
	}
	return true;
}