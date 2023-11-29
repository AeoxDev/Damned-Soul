#include "Relics\Offensive\CrackedLense.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define CRACKED_LENSE_INCREASE_STRENGTH (.25f)

EntityID CRACKED_LENSE::_OWNER;

const char* CRACKED_LENSE::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your Attack Damage by %ld%% of your Base Weapon Damage", PERCENT(CRACKED_LENSE_INCREASE_STRENGTH));
#pragma warning(suppress : 4172)
	return temp;
}

void CRACKED_LENSE::Initialize(void* input)
{
	// Set owner
	CRACKED_LENSE::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(CRACKED_LENSE::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(CRACKED_LENSE::IncreaseStrength);
}

void CRACKED_LENSE::IncreaseStrength(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == CRACKED_LENSE::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusDamage(CRACKED_LENSE_INCREASE_STRENGTH * stats->GetBaseDamage());
	}
}
