#include "Relics/DemonSkin.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_SKIN_DAMAGE_MULTIPLIER (0.8f)

EntityID DEMON_SKIN::_OWNER;

void DEMON_SKIN::Initialize(void* input)
{
	// Set owner
	DEMON_SKIN::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(DEMON_SKIN::DecreaseDamageTaken);
}

void DEMON_SKIN::DecreaseDamageTaken(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner
	if (input->defender.index == DEMON_SKIN::_OWNER.index)
	{
		input->decMult *= DEMON_SKIN_DAMAGE_MULTIPLIER;
	}
}
