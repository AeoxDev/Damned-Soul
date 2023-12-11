#include "Relics\Gadget\GoliathSkull.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define GOLIATH_SKULL_BONUS_HEALTH_FACTOR (0.06f)

EntityID GOLIATH_SKULL::_OWNER;

const char* GOLIATH_SKULL::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your attack damage by %ld%% of your bonus health", PERCENT(GOLIATH_SKULL_BONUS_HEALTH_FACTOR));
#pragma warning(suppress : 4172)
	return temp;
}

void GOLIATH_SKULL::Initialize(void* input)
{
	// Set owner
	GOLIATH_SKULL::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(GOLIATH_SKULL::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(GOLIATH_SKULL::IncreaseAttackDamage);
}

void GOLIATH_SKULL::IncreaseAttackDamage(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	static bool open = true;
	// Check if this is the owner
	if (open && input->entity.index == GOLIATH_SKULL::_OWNER.index)
	{
		// Lock out to prevent infinite loops
		open = false;

		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;

		StatComponent tempStats = *stats;
		tempStats.ZeroBonusStats();
		RelicInput::OnStatCalcInput temp;
		temp.entity = GOLIATH_SKULL::_OWNER;
		temp.adressOfStatComonent = &tempStats;
		for (auto func : GetFunctionsOfType(Relics::FUNC_ON_STAT_CALC))
			func(&temp);

		// Increase damage
		stats->UpdateBonusDamage(GOLIATH_SKULL_BONUS_HEALTH_FACTOR * tempStats.GetBonusHealth());

		// Unlock, so that bonuses can continue to be calculated
		open = true;
	}
}
