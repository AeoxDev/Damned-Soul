#include "Relics/DemonBonemarrow.h"
#include "Relics/RelicInternalHelper.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_BONEMARROW_STRENGTH_INCREASE 15

void DEMON_BONEMARROW::Initialize(void* input)
{	
	// Make sure the relic function map exists
	_validateRelicFunctions();
	// Call the increase strength function
	DEMON_BONEMARROW::IncreasePlayerStrength(nullptr);
	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(DEMON_BONEMARROW::IncreasePlayerStrength);
}

void DEMON_BONEMARROW::IncreasePlayerStrength(void* data)
{
	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		stats->damage += DEMON_BONEMARROW_STRENGTH_INCREASE;
	}
}
