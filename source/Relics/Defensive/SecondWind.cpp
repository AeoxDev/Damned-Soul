#include "Relics\Defensive\SecondWind.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "KnockBackComponent.h"

#define SECOND_WIND_REVIVE_FRACTION (.25f)

static bool _Second_Wind_Available = true;

EntityID SECOND_WIND::_OWNER;

void SECOND_WIND::Initialize(void* input)
{
	// Set owner
	SECOND_WIND::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Mark as available immediately
	_Second_Wind_Available = true;


	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_TAKEN].push_back(SECOND_WIND::Revive);
}

void SECOND_WIND::Revive(void* data)
{
	// Get the input
	RelicInput::OnHealthUpdate* input = (RelicInput::OnHealthUpdate*)data;
	StatComponent* stats = registry.GetComponent<StatComponent>(SECOND_WIND::_OWNER);

	// If this is the owner, and the owner now has 0 hp, set hp to the correct revive percentage
	if (_Second_Wind_Available && input->adressOfStatComponent == stats && stats->GetHealth() < 0.001f)
	{
		stats->ApplyHealing(stats->GetMaxHealth() * SECOND_WIND_REVIVE_FRACTION, false);
		_Second_Wind_Available = false;
	}
}
