#include "Relics/RelicInternalHelper.h"
#include "Relics/SpeedyLittleDevil.h"
#include "Registry.h"
#include "Components.h"

#define SPEEDY_LITTLE_DEVIL_SPEED_INCREASE (4.f)

void SPEEDY_LITTLE_DEVIL::Initialize(void* input)
{
	// Make sure the relic function map exists
	_validateRelicFunctions();
	// Call the increase speed function
	SPEEDY_LITTLE_DEVIL::IncreasePlayerSpeed(nullptr);
	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(SPEEDY_LITTLE_DEVIL::IncreasePlayerSpeed);
}

void SPEEDY_LITTLE_DEVIL::IncreasePlayerSpeed(void* data)
{
	EntityID player;
	for (auto entity : View<PlayerComponent>(registry))
		player = entity;

	StatComponent* stats = registry.GetComponent<StatComponent>(player);
	if (stats)
	{
		stats->moveSpeed += SPEEDY_LITTLE_DEVIL_SPEED_INCREASE;
	}
}
