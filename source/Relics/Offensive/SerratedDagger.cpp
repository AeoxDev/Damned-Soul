#include "Relics\Offensive\SerratedDagger.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SERRATED_DAGGER_RATE (1.f/3.f)

EntityID SERRATED_DAGGER::_OWNER;

const char* SERRATED_DAGGER::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Your charge attacks gain charge %ld%% faster",
		100 - PERCENT(1.f / (1.f + SERRATED_DAGGER_RATE)));
#pragma warning(suppress : 4172)
	return temp;
}

void SERRATED_DAGGER::Initialize(void* input)
{
	// Set owner
	SERRATED_DAGGER::_OWNER = *((EntityID*)input);

	// Mark as modified
	registry.GetComponent<StatComponent>(SERRATED_DAGGER::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_PLAYER_STAT_CALC].push_back(SERRATED_DAGGER::IncreaseChargeRate);
}

void SERRATED_DAGGER::IncreaseChargeRate(void* data)
{
	RelicInput::OnStatCalcInputPlayer* input = (RelicInput::OnStatCalcInputPlayer*)data;

	// Check if this is the owner
	if (input->entity.index == SERRATED_DAGGER::_OWNER.index)
	{
		// Get player component
		PlayerComponent* playerComp = (PlayerComponent*)input->adressOfPlayerComonent;

		// Increase dash distance based on speed
		playerComp->UpdateBonusChargeRate(SERRATED_DAGGER_RATE);
	}
}