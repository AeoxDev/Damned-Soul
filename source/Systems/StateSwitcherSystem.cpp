#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"
#include "States\StateManager.h"

bool StateSwitcherSystem::Update()
{
	PlayerComponent* statComp = nullptr;

	// Loop through registry to find an entity that has PlayerComponent AND StatComponent
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Get player entity stat component
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
		if (statComp != nullptr)
		{
			if (statComp->health <= 0)
			{
				stateManager.GetCurrentLevel().GameOver();
			}
		}
	}

	return true;
}