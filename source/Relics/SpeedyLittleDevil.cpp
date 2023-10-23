#include "Relics/SpeedyLittleDevil.h"
#include "Registry.h"
#include "Components.h"

#define SPEEDY_LITTLE_DEVIL_SPEED_INCREASE (4.f)

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
