#include "Relics/DemonBonemarrow.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_BONE_MARROW_HEALTH_INCREASE 25

void DEMON_BONEMARROW::IncreasePlayerHealth(void* data)
{
	// Should return only the player entity
	for (auto entity : View<ControllerComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		stats->health += DEMON_BONE_MARROW_HEALTH_INCREASE;
	}
}
