#include "Relics/SoulPower.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_POWER_SOUL_FACTOR (1.f)

void SOUL_POWER::Initialize(void* input)
{
	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Modify player strength by current souls
	// This effect is NOT added to the "On Obtain" list, since this isn't meant to be a one and done, but rather a setup for a constantly updating bonus
	SOUL_POWER::ModifyPlayerStrengthInitial(nullptr);

	// Add the "On Soul Update" function that takes the delta of souls and modifies Strength by it and adds it to the list
	(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SOUL_POWER::ModifyPlayerStrengthUpdate);
}

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