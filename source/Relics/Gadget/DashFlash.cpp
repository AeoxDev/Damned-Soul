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
	(*_RelicFunctions)[FUNC_ON_PLAYER_STAT_CALC].push_back(DASH_FLASH::IncreaseDashDistance);
}

void DASH_FLASH::IncreaseDashDistance(void* data)
{
	RelicInput::OnStatCalcInputPlayer* input = (RelicInput::OnStatCalcInputPlayer*)data;

	// Check if this is the owner
	if (input->entity.index == DASH_FLASH::_OWNER.index)
	{
		// Get player component
		PlayerComponent* playerComp = (PlayerComponent*)input->adressOfPlayerComonent;
		// Get  stats
		StatComponent* stats = registry.GetComponent<StatComponent>(DASH_FLASH::_OWNER);

		// Increase dash distance based on speed
		playerComp->UpdateBonusDashScaling(DASH_FLASH_FACTOR * stats->GetBonusSpeed());
	}
}