#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"
#include "EventFunctions.h"
#include "States\StateManager.h"

bool StateSwitcherSystem::Update()
{
	PlayerComponent* playersComp = nullptr;

	// Loop through registry to find an entity that has PlayerComponent AND StatComponent
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Get player entity stat component
		playersComp = registry.GetComponent<PlayerComponent>(entity);
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
		if (statComp != nullptr)
		{
			if (statComp->health <= 0)
			{
				stateManager.GetCurrentLevel().GameOver();
			}
		}
	}

	for (auto entity : View<EnemyComponent, StatComponent>(registry))
	{
		// Get enemy entity stat component
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
		if (statComp->health <= 0 && statComp->performingDeathAnimation == false)
		{
			statComp->performingDeathAnimation = true;
			if (playersComp != nullptr)
			{
				playersComp->killingSpree += 1;
			}
			// start timed event
			AddTimedEventComponentStartContinousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, RemoveEnemy);
		}
	}


	

	return true;
}