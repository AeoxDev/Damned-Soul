#include "Relics\Gadget\Discount\SuccubusCharm.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"

EntityID SUCCUBUS_CHARM::_OWNER;

#define SUCCUBUS_CHARM_COST_MULTIPLIER (.8f)
#define SUCCUBUS_CHARM_DAMAGE_REDUCTION (2)

const char* SUCCUBUS_CHARM::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Reduces the cost of healing and relics by %ld%% and reduces damage taken from attacks and hazards by %ld",
		100 - PERCENT(SUCCUBUS_CHARM_COST_MULTIPLIER),
		SUCCUBUS_CHARM_DAMAGE_REDUCTION);
#pragma warning(suppress : 4172)
	return temp;
}

void SUCCUBUS_CHARM::Initialize(void* input)
{
	// Set owner
	SUCCUBUS_CHARM::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of on on damage calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(SUCCUBUS_CHARM::DamageReduction);

	// Add it to the list of on price calc functions
	(*_RelicFunctions)[FUNC_ON_PRICE_CALC].push_back(SUCCUBUS_CHARM::Discount);
}

void SUCCUBUS_CHARM::DamageReduction(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner, and if its an approved damage type
	if (input->defender.index == SUCCUBUS_CHARM::_OWNER.index && (input->typeSource & (RelicInput::DMG::ENEMY | RelicInput::DMG::HAZARD)))
	{
		input->flatSub += SUCCUBUS_CHARM_DAMAGE_REDUCTION;
	}
}


void SUCCUBUS_CHARM::Discount(void* data)
{
	// Get the input
	RelicInput::OnPriceCalculation* input = (RelicInput::OnPriceCalculation*)data;

	input->healCostMult *= SUCCUBUS_CHARM_COST_MULTIPLIER;
	input->relicCostMult *= SUCCUBUS_CHARM_COST_MULTIPLIER;
}
