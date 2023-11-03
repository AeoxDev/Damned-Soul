#include "Relics/LifeSteal.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define LIFE_STEAL_HEALING_MULTIPLIER (0.15f)

EntityID LIFE_STEAL::_OWNER;

void LIFE_STEAL::Initialize(void* input)
{
	// Set owner
	LIFE_STEAL::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(LIFE_STEAL::HealFromDamage);
}

void LIFE_STEAL::HealFromDamage(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner
	if (input->attacker.index == LIFE_STEAL::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = registry.GetComponent<StatComponent>(_OWNER);

		// Heal from the damage dealt
		stats->UpdateHealth(LIFE_STEAL_HEALING_MULTIPLIER * input->CollapseDamage());
	}
}
