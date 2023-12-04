#include "Relics\Gadget\GaleAmulet.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"

#define GALE_AMULET_SPEED_INCREASE (.2f)
#define GALE_AMULET_ATTACK_SPEED_INCREASE (.2f)

EntityID GALE_AMULET::_OWNER;

const char* GALE_AMULET::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your speed by %ld%% and your attack speed by %ld%%",
		PERCENT(GALE_AMULET_SPEED_INCREASE),
		PERCENT(GALE_AMULET_ATTACK_SPEED_INCREASE));
#pragma warning(suppress : 4172)
	return temp;
}

void GALE_AMULET::Initialize(void* input)
{
	// Set owner
	GALE_AMULET::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(GALE_AMULET::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(GALE_AMULET::IncreaseSpeed);
}

void GALE_AMULET::IncreaseSpeed(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == GALE_AMULET::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;

		// Increase speeds
		stats->UpdateBonusSpeed(GALE_AMULET_SPEED_INCREASE);
		stats->UpdateBonusAttackSpeed(GALE_AMULET_ATTACK_SPEED_INCREASE);
	}
}
