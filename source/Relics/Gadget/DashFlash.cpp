#include "Relics\Gadget\DashFlash.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define DASH_FLASH_FACTOR (.15f)

EntityID DASH_FLASH::_OWNER;

const char* DASH_FLASH::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Your dash distance increases based on your Bonus Speed");
#pragma warning(suppress : 4172)
	return temp;
}

void DASH_FLASH::Initialize(void* input)
{
	// Set owner
	DASH_FLASH::_OWNER = *((EntityID*)input);

	// Mark as modified
	registry.GetComponent<StatComponent>(DASH_FLASH::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(DASH_FLASH::IncreaseDashDistance);
}

void DASH_FLASH::IncreaseDashDistance(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;
	static bool _enter = true;

	// Check if this is the owner
	if (input->entity.index == DASH_FLASH::_OWNER.index && _enter)
	{
		// Lock so that we don't get infinite recursive calls
		_enter = false;
		// Push temp stats onto the stack
		StatComponent* tempStats = (StatComponent*)MemLib::spush(sizeof(StatComponent));
		// Get real stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Set the temp stats
		*tempStats = *stats;
		tempStats->ZeroBonusStats();

		// Calculate stats
		RelicInput::OnStatCalcInput tempInput =
		{
			DASH_FLASH::_OWNER,
			tempStats
		};
		for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_STAT_CALC))
			func(&tempInput);

		// Increase dash distance based on speed
		stats->UpdateBonusDashDistance(DASH_FLASH_FACTOR * stats->GetBonusSpeed());

		// Pop and unlock
		MemLib::spop();
		_enter = true;
	}
}