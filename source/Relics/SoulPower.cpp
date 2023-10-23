#include "Relics/SoulPower.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_POWER_SOUL_FACTOR (1.f)

void SOUL_POWER::ModifyPlayerStrengthInitial(void* data)
{
	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Get stats
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		// Get souls
		PlayerComponent* playerData = registry.GetComponent<PlayerComponent>(entity);
		// Modify stats by souls
		stats->damage += SOUL_POWER_SOUL_FACTOR * playerData->GetSouls();
	}
}

void SOUL_POWER::ModifyPlayerStrengthUpdate(void* data)
{
	// Take the input for the soul update function
	RelicInput::OnSoulUpdate* input = (RelicInput::OnSoulUpdate*)data;

	// Should return only the player entity
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Check the soul delta
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		// Modify stats by soul delta
		stats->damage += SOUL_POWER_SOUL_FACTOR * input->soulDelta;
	}
}