#include "Relics\Gadget\Reckless.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define RECKLESS_DAMAGE_TAKEN_MULT (1.75f)
#define RECKLESS_DAMAGE_DEALT_MULT (1.5f)

EntityID RECKLESS::_OWNER;

const char* RECKLESS::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You deal %ld%% more damage, but take %ld%% more damage",
		PERCENT(RECKLESS_DAMAGE_DEALT_MULT) - 100,
		PERCENT(RECKLESS_DAMAGE_TAKEN_MULT)) - 100;
#pragma warning(suppress : 4172)
	return temp;
}

// Call to set active for a specific entity (usually player)
void RECKLESS::Initialize(void* input)
{
	// Set owner
	RECKLESS::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(RECKLESS::DamageAmplifier);
};


// The Increase Damage Dealt function for Reckless
void RECKLESS::DamageAmplifier(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner
	if (input->attacker.index == RECKLESS::_OWNER.index)
	{
		// Increase the damage dealt
		input->incMult *= RECKLESS_DAMAGE_DEALT_MULT;
	}
	else if (input->defender.index == RECKLESS::_OWNER.index)
	{
		// Increase the damage taken
		input->incMult *= RECKLESS_DAMAGE_TAKEN_MULT;
	}
}