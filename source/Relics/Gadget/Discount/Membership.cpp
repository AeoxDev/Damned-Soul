#include "Relics\Gadget\Discount\Membership.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"

EntityID MEMBERSHIP::_OWNER;

#define MEMBERSHIP_COST_MULT (.7f)

const char* MEMBERSHIP::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Reduces the cost of healing and upgrades by %ld%% while in the shop",
		100 - PERCENT(MEMBERSHIP_COST_MULT));
#pragma warning(suppress : 4172)
	return temp;
}

void MEMBERSHIP::Initialize(void* input)
{
	// Set owner
	MEMBERSHIP::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of on price calc functions
	(*_RelicFunctions)[FUNC_ON_PRICE_CALC].push_back(MEMBERSHIP::Discount);
}

void MEMBERSHIP::Discount(void* data)
{
	// Get the input
	RelicInput::OnPriceCalculation* input = (RelicInput::OnPriceCalculation*)data;

	input->healCostMult *= MEMBERSHIP_COST_MULT;
	input->upgradeCost *= MEMBERSHIP_COST_MULT;
}
