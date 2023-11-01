#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Skynet\BehaviourHelper.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Hitbox.h"



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

	tempBossTransformComponent->positionX += dirX * stats->GetSpeed() * GetDeltaTime();
	tempBossTransformComponent->positionZ += dirZ * stats->GetSpeed() * GetDeltaTime();
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


	tempBossTransformComponent->positionX += tempBossTransformComponent->facingX * stats->GetSpeed() / 2.f * GetDeltaTime();
	tempBossTransformComponent->positionZ += tempBossTransformComponent->facingZ * stats->GetSpeed() / 2.f * GetDeltaTime();

}

//void TemporaryPretendAnimation()
//{
//	// lmao funny code do many many things
//	int i = 0;
//}
//void BeginHitting(EntityID& entity)
//{
//	auto comp = registry.GetComponent<EnemyComponent>(entity);
//	SetHitboxActive(entity, comp->attackHitBoxID, true);
//	SetHitboxCanDealDamage(entity, comp->attackHitBoxID, true);
//}
//void WeShallOverCome(EntityID& entity)
//{
//	auto comp = registry.GetComponent<EnemyComponent>(entity);
//	SetHitboxActive(entity, comp->attackHitBoxID, false);
//	SetHitboxCanDealDamage(entity, comp->attackHitBoxID, false);
//}


void CombatBehaviour(TempBossBehaviour* bc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* btc, EnemyComponent* enmComp, EntityID& ent)
{
	bc->attackTimer += GetDeltaTime();
	bc->goalDirectionX = ptc->positionX - btc->positionX;
	bc->goalDirectionZ = ptc->positionZ - btc->positionZ;
	//Elliot & Herman request: Make animationtime scale better for faster startup and swing.
	//animComp->aAnim = ANIMATION_ATTACK;
	//animComp->aAnimIdx = 0;
	//Elliot: Change in calculations for attack timer:
	//animComp->aAnimTime = 0.5f * sc->attackTimer / (0.0001f + enemyStats->attackSpeed);
	//ANIM_BRANCHLESS(animComp);

	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (bc->attackTimer >= enemyStats->GetAttackSpeed()) // yes, we can indeed attack. 
	{
		//Set hitbox active here.
		//Elliot's request: Add Skeleton attack hitbox instead of define
		SetHitboxActive(ent, enmComp->attackHitBoxID, true);
		SetHitboxCanDealDamage(ent, enmComp->attackHitBoxID, true);
		//SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		//sfx->Play(Skeleton_Attack, Channel_Base);
		RedrawUI();
		bc->attackTimer = 0;
		bc->attackStunDurationCounter = 0;
	}
		//AddTimedEventComponentStartContinuousEnd(entity, 0.f, BeginHitting, TemporaryPretendAnimation, 0.3f, WeShallOverCome);
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
	EnemyComponent* enmComp = nullptr;

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
		enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);

		if (tempBossComponent != nullptr && playerTransformCompenent != nullptr && enmComp != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(tempBossTransformComponent->positionX, tempBossTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			tempBossComponent->attackStunDurationCounter += GetDeltaTime();

			if (tempBossComponent->attackStunDurationCounter <= tempBossComponent->attackStunDuration)
			{
				// do nothing, stand like a bad doggo and be ashamed
				//Elliot: When finished, reset attack timer and hitbox
				tempBossComponent->attackTimer = 0.0f;
				//enemyAnim->aAnimTime += (float)(enemyAnim->aAnimTime < 1.0f) * GetDeltaTime();
				continue;
			}
			else//Elliot: Turn off attack hitbox to not make player rage.
			{
				SetHitboxActive(enemyEntity, enmComp->attackHitBoxID, false);
				SetHitboxCanDealDamage(enemyEntity, enmComp->attackHitBoxID, false);
			}


			if (distance < 7.f - tempBossComponent->deathCounter * 1.15f || tempBossComponent->attackTimer > 0.0f)
			{
				CombatBehaviour(tempBossComponent, enemyStats, playerStats, playerTransformCompenent, tempBossTransformComponent, enmComp, enemyEntity);
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