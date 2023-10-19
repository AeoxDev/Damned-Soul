#include "Relics/DemonBonemarrow.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_BONEMARROW_STRENGTH_INCREASE 15

void DEMON_BONEMARROW::IncreasePlayerStrength(void* data)
{
	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		stats->damage += DEMON_BONEMARROW_STRENGTH_INCREASE;
	}
}
