#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Skynet\BehaviourHelper.h"
#include "ParticleComponent.h"
#include "Particles.h"



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

		frozenComponent->frozenCounter += GetDeltaTime();
	}

	return true;
}