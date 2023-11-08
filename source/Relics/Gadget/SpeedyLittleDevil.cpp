#include "Relics\Gadget\SpeedyLittleDevil.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"

#define SPEEDY_LITTLE_DEVIL_SPEED_INCREASE (4.f)

EntityID SPEEDY_LITTLE_DEVIL::_OWNER;

void SPEEDY_LITTLE_DEVIL::Initialize(void* input)
{
	// Set owner
	SPEEDY_LITTLE_DEVIL::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(SPEEDY_LITTLE_DEVIL::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(SPEEDY_LITTLE_DEVIL::IncreaseSpeed);
}

void SPEEDY_LITTLE_DEVIL::IncreaseSpeed(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == SPEEDY_LITTLE_DEVIL::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusSpeed(SPEEDY_LITTLE_DEVIL_SPEED_INCREASE);
	}
}
