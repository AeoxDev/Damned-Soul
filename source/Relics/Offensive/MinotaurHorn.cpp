#include "Relics\Offensive\MinotaurHorn.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define MINOTAUR_HORN_STRENGTH_INCREASE (.2f)
#define MINOTAUR_HORN_KNOCKBACK_INCREASE (.3f)

EntityID MINOTAUR_HORN::_OWNER;

const char* MINOTAUR_HORN::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf(temp, "Increases your strength by %ld and your knockback force by %ld%%",
		PERCENT(MINOTAUR_HORN_STRENGTH_INCREASE),
		PERCENT(MINOTAUR_HORN_KNOCKBACK_INCREASE));
	return temp;
}

void MINOTAUR_HORN::Initialize(void* input)
{
	// Set owner
	MINOTAUR_HORN::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(MINOTAUR_HORN::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(MINOTAUR_HORN::IncreaseStrengthAndKnockback);
}

void MINOTAUR_HORN::IncreaseStrengthAndKnockback(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == MINOTAUR_HORN::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusDamage(MINOTAUR_HORN_STRENGTH_INCREASE * stats->GetBaseDamage());
		stats->UpdateBonusKnockback(MINOTAUR_HORN_KNOCKBACK_INCREASE);
	}
}
