#include "Relics/RelicInternalHelper.h"
#include "Relics/DemonHeart.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_HEART_HEALTH_INCREASE 75

void DEMON_HEART::Initialize(void* input)
{
	// Make sure the relic function map exists
	_validateRelicFunctions();
	// Call the increase health function
	DEMON_HEART::IncreasePlayerHealth(nullptr);
	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(DEMON_HEART::IncreasePlayerHealth);
}

void DEMON_HEART::IncreasePlayerHealth(void* data)
{
	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		stats->UpdateMaxHealth(DEMON_HEART_HEALTH_INCREASE);
	}
}
