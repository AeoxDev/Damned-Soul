#include "Relics/SoulHealth.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_HEALTH_SOUL_FACTOR (1.f)

void SOUL_HEALTH::ModifyPlayerHealthInitial(void* data)
{
	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Get stats
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		// Get souls
		PlayerComponent* playerData = registry.GetComponent<PlayerComponent>(entity);
		// Modify stats by souls
		stats->UpdateMaxHealth(SOUL_HEALTH_SOUL_FACTOR * playerData->GetSouls());
	}
}

void SOUL_HEALTH::ModifyPlayerHealthUpdate(void* data)
{
	// Take the input for the soul update function
	RelicInput::OnSoulUpdate* input = (RelicInput::OnSoulUpdate*)data;

	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Check the soul delta
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		// Modify stats by soul delta
		stats->UpdateMaxHealth(SOUL_HEALTH_SOUL_FACTOR * input->soulDelta);
	}
}
