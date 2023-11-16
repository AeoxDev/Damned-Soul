#include "Relics\Offensive\LifeSteal.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define LIFE_STEAL_HEALING_MULTIPLIER (0.15f)

EntityID LIFE_STEAL::_OWNER;

const char* LIFE_STEAL::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You heal for %ld%% of the attack damage you deal", PERCENT(LIFE_STEAL_HEALING_MULTIPLIER));
	return temp;
}

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
		stats->ApplyHealing(LIFE_STEAL_HEALING_MULTIPLIER * input->CollapseDamage());
	}
}
