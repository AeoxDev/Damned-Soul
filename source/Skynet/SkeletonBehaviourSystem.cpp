#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>

#define SKELETON_ATTACK_HITBOX 2

void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent, StatComponent* stats, AnimationComponent* animComp)
{
	skeletonComponent->goalDirectionX = playerTransformCompenent->positionX - skeletonTransformComponent->positionX;
	skeletonComponent->goalDirectionZ = playerTransformCompenent->positionZ - skeletonTransformComponent->positionZ;

	animComp->aAnim = ANIMATION_WALK;
	animComp->aAnimIdx = 0;
	animComp->aAnimTime += GetDeltaTime();
	// Loop back
	while (1.f < animComp->aAnimTime)
		animComp->aAnimTime -= 1.f;

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

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent, StatComponent* stats, AnimationComponent* animComp)
{
	skeletonComponent->timeCounter += GetDeltaTime();

	animComp->aAnim = ANIMATION_IDLE;
	animComp->aAnimIdx = 0;
	animComp->aAnimTime += GetDeltaTime();
	// Loop back
	while (1.f < animComp->aAnimTime)
		animComp->aAnimTime -= 1.f;

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


	skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * stats->moveSpeed / 2.f * GetDeltaTime();
	skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * stats->moveSpeed / 2.f * GetDeltaTime();

}
void CombatBehaviour(SkeletonBehaviour* sc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* stc, EntityID& ent, AnimationComponent* animComp)
{
	sc->attackTimer += GetDeltaTime() * enemyStats->attackSpeed;
	sc->goalDirectionX = ptc->positionX - stc->positionX;
	sc->goalDirectionZ = ptc->positionZ - stc->positionZ;

	//Elliot & Herman request: Make animationtime scale better for faster startup and swing.
	animComp->aAnim = ANIMATION_ATTACK;
	animComp->aAnimIdx = 0;
	//Elliot: Change in calculations for attack timer:
	animComp->aAnimTime = 0.5f * sc->attackTimer / (0.0001f + enemyStats->attackSpeed);
	while (1.f < animComp->aAnimTime)
	{
		animComp->aAnimTime -= 1.f;
	}
		

	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (sc->attackTimer >= enemyStats->attackSpeed) // yes, we can indeed attack. 
	{
		//Set hitbox active here.
		//Elliot's request: Add Skeleton attack hitbox instead of define
		SetHitboxActive(ent, sc->attackHitboxID, true);
		SetHitboxCanDealDamage(ent, sc->attackHitboxID, true);
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		sfx->Play(Skeleton_Attack, Channel_Base);
		RedrawUI();
		sc->attackTimer = 0;
		sc->attackStunDurationCounter = 0;
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
	AnimationComponent* enemyAnim = nullptr;

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
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

		if (skeletonComponent != nullptr && playerTransformCompenent!= nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(skeletonTransformComponent->positionX, skeletonTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			skeletonComponent->attackStunDurationCounter += GetDeltaTime();
			if (skeletonComponent->attackStunDurationCounter <= skeletonComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
				//Elliot: When finished, reset attack timer and hitbox
				skeletonComponent->attackTimer = 0.0f;
				enemyAnim->aAnimTime += (float)(enemyAnim->aAnimTime < 1.0f) * GetDeltaTime();
				continue;
			}
			else//Elliot: Turn off attack hitbox to not make player rage.
			{
				SetHitboxActive(enemyEntity, skeletonComponent->attackHitboxID, false);
				SetHitboxCanDealDamage(enemyEntity, skeletonComponent->attackHitboxID, false);
			}


			//Elliot: If in attack, keep attacking even if player is outside
			if (distance < 2.5f || skeletonComponent->attackTimer > 0.0f)
			{
				CombatBehaviour(skeletonComponent, enemyStats, playerStats, playerTransformCompenent, skeletonTransformComponent, enemyEntity, enemyAnim);
			}
			else if (distance < 50.f) //hunting distance
			{
				
				ChaseBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent, enemyStats, enemyAnim);
			}
		}
	}


	return true;
}