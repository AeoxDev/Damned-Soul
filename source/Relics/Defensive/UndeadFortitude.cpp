#include "Relics\Defensive\UndeadFortitude.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include <cmath>

#define UNDEAD_FORTITUDE_MINIMUM_MULTIPLIER (0.9f)
#define UNDEAD_FORTITUDE_MAXIMUM_MULTIPLIER (0.6f)

EntityID UNDEAD_FORTITUDE::_OWNER;

void UNDEAD_FORTITUDE::Initialize(void* input)
{
	// Set owner
	UNDEAD_FORTITUDE::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(UNDEAD_FORTITUDE::DecreaseDamageTaken);
}

void UNDEAD_FORTITUDE::DecreaseDamageTaken(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner
	if (input->defender.index == UNDEAD_FORTITUDE::_OWNER.index)
	{
		auto stats = registry.GetComponent<StatComponent>(UNDEAD_FORTITUDE::_OWNER);

		// Calculate reduction, increasing with missing health
		float finalMod = std::lerp(UNDEAD_FORTITUDE_MAXIMUM_MULTIPLIER, UNDEAD_FORTITUDE_MINIMUM_MULTIPLIER, stats->GetHealthFraction());

		// Decrease damage taken based on missing health
		input->decMult *= finalMod;
	}
}
