#include "Relics\Offensive\HoundFang.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define HOUND_FANG_BASE_DAMAGE_INCREASE (3)

EntityID HOUND_FANG::_OWNER;

const char* HOUND_FANG::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf(temp, "Increases your weapon damage by %ld flat", HOUND_FANG_BASE_DAMAGE_INCREASE);
	return temp;
}

void HOUND_FANG::Initialize(void* input)
{
	// Set owner
	HOUND_FANG::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(HOUND_FANG::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(HOUND_FANG::IncreaseBaseDamage);
}

void HOUND_FANG::IncreaseBaseDamage(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == HOUND_FANG::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusDamage(HOUND_FANG_BASE_DAMAGE_INCREASE);
	}
}
