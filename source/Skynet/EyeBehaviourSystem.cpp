#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include "UI/UIRenderer.h"
#include <random>


void RetreatBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats)
{
	//calculate the direction away from the player
	eyeComponent->goalDirectionX = -(playerTransformCompenent->positionX - eyeTransformComponent->positionX);
	eyeComponent->goalDirectionZ = -(playerTransformCompenent->positionZ - eyeTransformComponent->positionZ);
	float magnitude = sqrt(eyeComponent->goalDirectionX * eyeComponent->goalDirectionX + eyeComponent->goalDirectionZ * eyeComponent->goalDirectionZ);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}
	eyeComponent->goalDirectionX /= magnitude;
	eyeComponent->goalDirectionZ /= magnitude;
	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);
	float dirX = eyeTransformComponent->facingX, dirZ = eyeTransformComponent->facingZ;
	magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
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
	float magnitude = 0.f;
	float dirX = 0.f;
	float dirZ = 0.f;

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
	SmoothRotation(etc, dirX, dirZ, 10.f);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}
	dirX /= magnitude;
	dirZ /= magnitude;
	etc->positionX += dirX * enemyStats->moveSpeed * GetDeltaTime();
	etc->positionZ += dirZ * enemyStats->moveSpeed * GetDeltaTime();
	ec->goalDirectionX = ptc->positionX - etc->positionX;
	ec->goalDirectionZ = ptc->positionZ - etc->positionZ;
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats)
{
	/*eyeComponent->timeCounter += GetDeltaTime();
	if (eyeComponent->timeCounter >= eyeComponent->updateInterval)
	{
		eyeComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());

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
	eyeTransformComponent->positionZ += eyeTransformComponent->facingZ * enemyStats->moveSpeed / 2.f * GetDeltaTime();*/
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

bool Collision(float aX, float aZ, float bX, float bZ, float tolerance) // A = enemy pos, B = player pos, tolerance = 
{
	if (aX < bX + tolerance && aX > bX - tolerance &&
		aZ < bZ + tolerance && aZ > bZ - tolerance)
		return true;
	else
		return false;
}

void ChargeBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats, StatComponent* playerStats, HitboxComponent* enemyHitbox, EntityID eID)
{
	if (!eyeComponent->charging)
	{
		eyeComponent->specialCounter = 0;
		eyeComponent->charging = true;

		//while charging disable hitboxes
		SetHitboxActive(eID, 0, false);
		SetHitboxActive(eID, 1, false);

		//direction from the enemy towards the player
		float dirX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
		float dirZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

		float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}

		dirX /= magnitude;
		dirZ /= magnitude;

		//target is the stopping point of the charge, a set distance behind the players position
		eyeComponent->targetX = playerTransformCompenent->positionX + dirX * 10.0f;
		eyeComponent->targetZ = playerTransformCompenent->positionZ + dirZ * 10.0f;

		eyeComponent->changeDirX = dirX; // charge direction is only set once per charge and will not change
		eyeComponent->changeDirZ = dirZ; 

		//change what direction the eye is circling after each dash
		(eyeComponent->clockwiseCircle == true) ? eyeComponent->clockwiseCircle = false : eyeComponent->clockwiseCircle = true;

		SmoothRotation(eyeTransformComponent, eyeComponent->changeDirX, eyeComponent->changeDirZ, 20.0f);
	}
	else 
	{
		//calculate the current direction towards player
		float dirX = (eyeComponent->targetX - eyeTransformComponent->positionX);
		float dirZ = (eyeComponent->targetZ - eyeTransformComponent->positionZ);

		float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		dirX /= magnitude;
		dirZ /= magnitude;

		//scalar between the current direction and the original chagre direction
		float scalar = dirX * eyeComponent->changeDirX + dirZ * eyeComponent->changeDirZ;

		//If charging scalar point direction > 0.0, charge
		if (scalar > 0)
		{
			SmoothRotation(eyeTransformComponent, eyeComponent->changeDirX, eyeComponent->changeDirZ, 10.0f);

			eyeTransformComponent->positionX += eyeComponent->changeDirX * enemyStats->moveSpeed * 6.f * GetDeltaTime();
			eyeTransformComponent->positionZ += eyeComponent->changeDirZ * enemyStats->moveSpeed * 6.f * GetDeltaTime();

			//check if eye has collided with player
			if (eyeComponent->dealtDamage == false && Collision(eyeTransformComponent->positionX, eyeTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ, 0.2f))
			{
				playerStats->UpdateHealth(-enemyStats->damage);
				eyeComponent->dealtDamage = true;
				RedrawUI();
			}
		}
		else //else charge is finished
		{
			//reenable hitboxes
			SetHitboxActive(eID, 0, true);
			SetHitboxActive(eID, 1, true);

			//reset values
			eyeComponent->charging = false;
			eyeComponent->attackTimer = 0;
			eyeComponent->attackStunDurationCounter = 0;
			eyeComponent->dealtDamage = false;
		}
	}
}

//static int TEMPCOUNTER_WILLBEREMOVEDLATER = 0; //used to increase the special counter, should be in the combatbehaviour but not yet implemented

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


		if (enemyStats->GetHealth() > 0 && eyeComponent != nullptr && playerTransformCompenent != nullptr && enemyHitbox != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(eyeTransformComponent->positionX, eyeTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			eyeComponent->attackTimer += GetDeltaTime();
			eyeComponent->attackStunDurationCounter += GetDeltaTime();
			if (eyeComponent->attackStunDurationCounter <= eyeComponent->attackStunDuration)
			{
				// do nothing, stand still and be ashamed
			}
			else if (distance < 25.0f && !eyeComponent->charging) // Retreat to safe distance if not charging
			{
				RetreatBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
			}
			else if (eyeComponent->charging || (eyeComponent->attackTimer > enemyStats->attackSpeed && distance < 45.f)/*eyeComponent->specialCounter > eyeComponent->specialBreakpoint*/) //if special is ready or is currently doing special
			{
				//CHAAAAARGE
				
				ChargeBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyHitbox, enemyEntity);

			}
			else if (distance <= 45.0f + eyeComponent->circleBehaviour) // circle player & attack when possible (WIP)
			{
				//SmoothRotation(eyeTransformComponent, eyeComponent->facingX, eyeComponent->facingZ);
				//if(!CombatBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats))
				CircleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats);
				
				//TEMPCOUNTER_WILLBEREMOVEDLATER++; //this will not be neccessary later
				//if (TEMPCOUNTER_WILLBEREMOVEDLATER % 1000 == 0)
				//{
				//	eyeComponent->specialCounter++;
				//}
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats);
			}
		}
	}

	
	return true;
}