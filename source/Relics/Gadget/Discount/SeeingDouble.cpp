#include "Relics\Gadget\Discount\SeeingDouble.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"

EntityID SEEING_DOUBLE::_OWNER;

// Gets assigned later, cost should round up, so the prices should be a nic and cozy zero
#define SEEING_DOUBLE_COST_MULT (-.01f)

static bool _Seeing_Double_Active = false;

const char* SEEING_DOUBLE::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Your next relic, upgrade, heal, or reroll in this shop is free!");
#pragma warning(suppress : 4172)
	return temp;
}

void SEEING_DOUBLE::Initialize(void* input)
{
	// Set owner
	SEEING_DOUBLE::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Set the relic to active
	_Seeing_Double_Active = true;

	// Add disable function to level swap and soul update lists!
	(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SEEING_DOUBLE::Disable);
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(SEEING_DOUBLE::Disable);

	// Add it to the list of on price calc functions
	(*_RelicFunctions)[FUNC_ON_PRICE_CALC].push_back(SEEING_DOUBLE::NoCostOnThings);
}

void SEEING_DOUBLE::Disable(void* data)
{
	_Seeing_Double_Active = false;
}

void SEEING_DOUBLE::NoCostOnThings(void* data)
{
	// Get the input
	RelicInput::OnPriceCalculation* input = (RelicInput::OnPriceCalculation*)data;

	if (_Seeing_Double_Active)
		// Assignment is by design!
		input->everythingCostMult = SEEING_DOUBLE_COST_MULT;
}
