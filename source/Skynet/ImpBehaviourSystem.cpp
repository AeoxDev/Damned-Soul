#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include "UI/UIRenderer.h"
#include <random>


void RetreatBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, ImpBehaviour* impComponent, TransformComponent* impTransformComponent, StatComponent* enemyStats, AnimationComponent* enemyAnim)
{
	// Regular walk
	enemyAnim->aAnim = ANIMATION_WALK;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime();
	ANIM_BRANCHLESS(enemyAnim);

	//calculate the direction away from the player
	impComponent->goalDirectionX = -(playerTransformCompenent->positionX - impTransformComponent->positionX);
	impComponent->goalDirectionZ = -(playerTransformCompenent->positionZ - impTransformComponent->positionZ);
	float magnitude = sqrt(impComponent->goalDirectionX * impComponent->goalDirectionX + impComponent->goalDirectionZ * impComponent->goalDirectionZ);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}
	impComponent->goalDirectionX /= magnitude;
	impComponent->goalDirectionZ /= magnitude;
	SmoothRotation(impTransformComponent, impComponent->goalDirectionX, impComponent->goalDirectionZ, 30.f);
	float dirX = impTransformComponent->facingX, dirZ = impTransformComponent->facingZ;
	magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}

	impTransformComponent->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
	impTransformComponent->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
}

bool CombatBehaviour(EntityID entity, PlayerComponent*& pc, TransformComponent*& ptc, ImpBehaviour*& ec, TransformComponent*& etc, StatComponent*& enemyStats, StatComponent*& playerStats, AnimationComponent* enemyAnim)
{
	//if you just attacked go back to circle behaviour
	if (ec->attackTimer < enemyStats->GetAttackSpeed())
	{
		ec->attackTimer += GetDeltaTime();
		return false;
	}
	//rotate imp in order to shoot at the player
	else if (ec->aimTimer < ec->aimDuration)
	{
		ec->goalDirectionX = ptc->positionX - etc->positionX;
		ec->goalDirectionZ = ptc->positionZ - etc->positionZ;
		float magnitude = sqrt(ec->goalDirectionX * ec->goalDirectionX + ec->goalDirectionZ * ec->goalDirectionZ);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		ec->goalDirectionX /= magnitude;
		ec->goalDirectionZ /= magnitude;


		SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
		ec->aimTimer += GetDeltaTime();
		ec->attackTimer += GetDeltaTime();
		return true;
	}
	else // yes, we can indeed attack. 
	{
		enemyAnim->aAnim = ANIMATION_ATTACK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
		ANIM_BRANCHLESS(enemyAnim);

		ec->attackTimer = 0;
		ec->attackStunDurationCounter = 0;
		ec->aimTimer = 0;
		ec->specialCounter++; //increase the special counter for special attack

		//set direction for attack
		float dx = (ptc->positionX - etc->positionX);
		float dz = (ptc->positionZ - etc->positionZ);

		//normalize initial direction
		float magnitude = sqrt(dx * dx + dz * dz);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}

		dx /= magnitude;
		dz /= magnitude;

		//		C		= sqrt(A^2 + B^2)
		float newDirX = sqrt(ptc->facingX * ptc->facingX + dx * dx);
		float newDirZ = sqrt(ptc->facingZ * ptc->facingZ + dz * dz);

		//how much the player moved since last frame
		float playerMovementX = ptc->positionX - ptc->lastPositionX;
		float playerMovementZ = ptc->positionZ - ptc->lastPositionZ;

		//limit player movement in case of dash
		if (playerMovementX > 0.02f)
			playerMovementX = 0.02f;
		if (playerMovementZ > 0.02f)
			playerMovementZ = 0.02f;

		//calculate final direction based on how much the player moved plus the player movespeed
		dx += newDirX * playerMovementX * playerStats->GetSpeed();
		dz += newDirZ * playerMovementZ * playerStats->GetSpeed();

		//normalize finial direction
		magnitude = sqrt(dx * dx + dz * dz);
		if (magnitude < 0.001f)
		{
			magnitude = 0.001f;
		}
		dx /= magnitude;
		dz /= magnitude;

		CreateProjectile(entity, dx, dz);
		return true;
	}
}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, ImpBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats, StatComponent* playerStats, AnimationComponent* enemyAnim)
{
	// Regular attack?
	enemyAnim->aAnim = ANIMATION_WALK;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime();
	ANIM_BRANCHLESS(enemyAnim);

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
	SmoothRotation(etc, dirX, dirZ, 30.f);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}
	dirX /= magnitude;
	dirZ /= magnitude;
	etc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
	etc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
	ec->goalDirectionX = ptc->positionX - etc->positionX;
	ec->goalDirectionZ = ptc->positionZ - etc->positionZ;
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, ImpBehaviour* impComponent, TransformComponent* impTransformComponent, StatComponent* enemyStats, AnimationComponent* enemyAnim)
{
	enemyAnim->aAnim = ANIMATION_IDLE;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime();
	ANIM_BRANCHLESS(enemyAnim);

	impComponent->timeCounter += GetDeltaTime();
	if (impComponent->timeCounter >= impComponent->updateInterval)
	{
		impComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		impComponent->goalDirectionX = randomX;
		impComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		impComponent->updateInterval = randomInterval(gen);
	}

	SmoothRotation(impTransformComponent, impComponent->goalDirectionX, impComponent->goalDirectionZ, 30.f);


	impTransformComponent->positionX += impTransformComponent->facingX * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
	impTransformComponent->positionZ += impTransformComponent->facingZ * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
}


void RepositionBehaviour();
bool ImpBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	ImpBehaviour* impComponent = nullptr;
	TransformComponent* impTransformComponent = nullptr;
	HitboxComponent* enemyHitbox = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enemComp = nullptr;
	//Find available entity


	for (auto enemyEntity : View<ImpBehaviour, TransformComponent, HitboxComponent, EnemyComponent>(registry))
	{

		SetLightColor(enemyEntity, 0.3f, 0.3f, 0.3f);
		impComponent = registry.GetComponent<ImpBehaviour>(enemyEntity);
		impTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		enemyHitbox = registry.GetComponent<HitboxComponent>(enemyEntity);
		enemComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		AnimationComponent* enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);
		//Find a player to kill.
		if (enemComp->lastPlayer.index == -1)
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
				enemComp->lastPlayer = playerEntity;
				if (rand() % 2)
				{
					break;
				}
			}
		}
		else
		{
			playerComponent = registry.GetComponent<PlayerComponent>(enemComp->lastPlayer);
			playerTransformCompenent = registry.GetComponent<TransformComponent>(enemComp->lastPlayer);
			playerStats = registry.GetComponent< StatComponent>(enemComp->lastPlayer);
			if (playerComponent == nullptr)
			{
				enemComp->lastPlayer.index = -1;
			}
		}


		if (enemyStats->GetHealth() > 0 && impComponent != nullptr && playerTransformCompenent != nullptr && enemyHitbox != nullptr && enemComp != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(impTransformComponent->positionX, impTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);

			impComponent->attackStunDurationCounter += GetDeltaTime();
			if (impComponent->attackStunDurationCounter <= impComponent->attackStunDuration)
			{
				// Regular attack?
				enemyAnim->aAnim = ANIMATION_IDLE;
				enemyAnim->aAnimIdx = 0;
				enemyAnim->aAnimTime += GetDeltaTime() * .7f;
				ANIM_BRANCHLESS(enemyAnim);
			}
			else if (distance < 25.0f && !impComponent->charging) // try to retreat to a safe distance if not charging
			{
				RetreatBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim);
			}
			else if (impComponent->specialCounter >= impComponent->specialBreakpoint) //if special is ready teleport
			{
				impComponent->specialCounter = 0;
				RepositionBehaviour();
			}
			else if (distance <= 45.0f + impComponent->circleBehaviour) // circle player & attack when possible (WIP)
			{
				if (!CombatBehaviour(enemyEntity, playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, playerStats, enemyAnim))
					CircleBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, playerStats, enemyAnim);
			}
			else // idle
			{
				enemComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim);
			}
		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enemComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(playerComponent, playerTransformCompenent, impComponent, impTransformComponent, enemyStats, enemyAnim);
		}
	}


	return true;
}