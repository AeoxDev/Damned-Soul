#include "Relics/DemonHeart.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_HEART_HEALTH_INCREASE 25

void DEMON_HEART::IncreasePlayerHealth(void* data)
{
	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		stats->UpdateMaxHealth(DEMON_HEART_HEALTH_INCREASE);
	}
}
