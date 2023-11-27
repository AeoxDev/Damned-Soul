#include "Relics\Gadget\QuickReflexes.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define DOUBLE_TROUBLE_EXTRA_DASHES (1)

EntityID QUICK_REFLEXES::_OWNER;

const char* QUICK_REFLEXES::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You can dash %ld additional time",
		DOUBLE_TROUBLE_EXTRA_DASHES);
#pragma warning(suppress : 4172)
	return temp;
}

void QUICK_REFLEXES::Initialize(void* input)
{
	// Set owner
	QUICK_REFLEXES::_OWNER = *((EntityID*)input);

	// Mark as modified
	registry.GetComponent<StatComponent>(QUICK_REFLEXES::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_PLAYER_STAT_CALC].push_back(QUICK_REFLEXES::IncreaseDashCount);
}

void QUICK_REFLEXES::IncreaseDashCount(void* data)
{
	RelicInput::OnStatCalcInputPlayer* input = (RelicInput::OnStatCalcInputPlayer*)data;

	// Check if this is the owner
	if (input->entity.index == QUICK_REFLEXES::_OWNER.index)
	{
		// Get player component
		PlayerComponent* playerComp = (PlayerComponent*)input->adressOfPlayerComonent;

		// Increase dash distance based on speed
		playerComp->UpdateBonusDashes(DOUBLE_TROUBLE_EXTRA_DASHES);
	}
}