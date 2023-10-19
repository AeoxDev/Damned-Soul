#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>



void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, TempBossBehaviour* tempBossComponent, TransformComponent* tempBossTransformComponent, StatComponent* stats)
{
	tempBossComponent->goalDirectionX = playerTransformCompenent->positionX - tempBossTransformComponent->positionX;
	tempBossComponent->goalDirectionZ = playerTransformCompenent->positionZ - tempBossTransformComponent->positionZ;

	SmoothRotation(tempBossTransformComponent, tempBossComponent->goalDirectionX, tempBossComponent->goalDirectionZ);
	float dirX = tempBossTransformComponent->facingX, dirZ = tempBossTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	tempBossTransformComponent->positionX += dirX * stats->moveSpeed * GetDeltaTime();
	tempBossTransformComponent->positionZ += dirZ * stats->moveSpeed * GetDeltaTime();
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, TempBossBehaviour* tempBossComponent, TransformComponent* tempBossTransformComponent, StatComponent* stats)
{
	tempBossComponent->timeCounter += GetDeltaTime();
	if (tempBossComponent->timeCounter >= tempBossComponent->updateInterval)
	{
		tempBossComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		tempBossComponent->goalDirectionX = randomX;
		tempBossComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		tempBossComponent->updateInterval = randomInterval(gen);
	}

	SmoothRotation(tempBossTransformComponent, tempBossComponent->goalDirectionX, tempBossComponent->goalDirectionZ);


	tempBossTransformComponent->positionX += tempBossTransformComponent->facingX * stats->moveSpeed / 2.f * GetDeltaTime();
	tempBossTransformComponent->positionZ += tempBossTransformComponent->facingZ * stats->moveSpeed / 2.f * GetDeltaTime();

}

void CombatBehaviour(TempBossBehaviour* sc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* btc)
{
	sc->goalDirectionX = ptc->positionX - btc->positionX;
	sc->goalDirectionZ = ptc->positionZ - btc->positionZ;

	SmoothRotation(btc, sc->goalDirectionX, sc->goalDirectionZ);
	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (sc->attackTimer >= enemyStats->attackSpeed) // yes, we can indeed attack. 
	{
		sc->attackTimer = 0;
		sc->attackStunDurationCounter = 0;
		playerStats->UpdateHealth(-enemyStats->damage);
		RedrawUI();
	}
}

bool TempBossBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	TempBossBehaviour* tempBossComponent = nullptr;
	TransformComponent* tempBossTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;

	for (auto playerEntity : View<PlayerComponent, TransformComponent, StatComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
		playerStats = registry.GetComponent< StatComponent>(playerEntity);
	}

	for (auto enemyEntity : View<TempBossBehaviour, TransformComponent, StatComponent>(registry))
	{
		tempBossComponent = registry.GetComponent<TempBossBehaviour>(enemyEntity);
		tempBossTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);

		if (tempBossComponent != nullptr && playerTransformCompenent != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(tempBossTransformComponent->positionX, tempBossTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			tempBossComponent->attackTimer += GetDeltaTime();
			tempBossComponent->attackStunDurationCounter += GetDeltaTime();

			if (tempBossComponent->attackStunDurationCounter <= tempBossComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
			}
			else if (distance < 7.f - tempBossComponent->deathCounter * 2.f)
			{
				CombatBehaviour(tempBossComponent, enemyStats, playerStats, playerTransformCompenent, tempBossTransformComponent);
			}
			else if (distance < 50) //hunting distance
			{
				ChaseBehaviour(playerComponent, playerTransformCompenent, tempBossComponent, tempBossTransformComponent, enemyStats);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, tempBossComponent, tempBossTransformComponent, enemyStats);
			}
		}
	}


	return true;
}