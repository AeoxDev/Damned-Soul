#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Skynet\BehaviourHelper.h"
#include "ParticleComponent.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"
#include "EventFunctions.h"

bool FrozenBehaviourSystem::Update()
{
	FrozenBehaviour* frozenComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	TransformComponent* frozenTransformComponent = nullptr;

	for (auto enemyEntity : View<FrozenBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		frozenComponent = registry.GetComponent<FrozenBehaviour>(enemyEntity);
		frozenTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		bool thanosSnap = false;
		frozenComponent->frozenCounter += GetDeltaTime();
		
		if (frozenComponent->frozenCounter >= frozenComponent->frozenTimeAmount)
		{
			if (enemyStats->GetHealth() > 0)
			{
				thanosSnap = true;
				if (frozenComponent->type == EnemyType::frozenHellhound)
				{
					SetupEnemy(EnemyType::hellhound, frozenTransformComponent->positionX, frozenTransformComponent->positionY, frozenTransformComponent->positionZ,
						6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 0);
				}
				else if (frozenComponent->type == EnemyType::frozenImp)
				{
					SetupEnemy(EnemyType::imp, frozenTransformComponent->positionX, frozenTransformComponent->positionY, frozenTransformComponent->positionZ,
						6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 0);
				}
				else if (frozenComponent->type == EnemyType::frozenEye)
				{
					SetupEnemy(EnemyType::eye, frozenTransformComponent->positionX, frozenTransformComponent->positionY, frozenTransformComponent->positionZ,
						6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 0);
				}
			}
		}
		if (thanosSnap)
		{
			RemoveEnemy(enemyEntity, 1);
		}
	}

	return true;
}