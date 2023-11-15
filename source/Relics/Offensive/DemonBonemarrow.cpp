#include "Relics\Offensive\DemonBonemarrow.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "MemLib\ML_String.hpp"

#define DEMON_BONEMARROW_STRENGTH_INCREASE (.25f)

EntityID DEMON_BONEMARROW::_OWNER;

const char* DEMON_BONEMARROW::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf(temp, "Increases your strength by %ld", PERCENT(DEMON_BONEMARROW_STRENGTH_INCREASE));
	return temp;
}

void DEMON_BONEMARROW::Initialize(void* input)
{	
	// Set owner
	DEMON_BONEMARROW::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(DEMON_BONEMARROW::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(DEMON_BONEMARROW::IncreasePlayerStrength);
}

void DEMON_BONEMARROW::IncreasePlayerStrength(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == DEMON_BONEMARROW::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusDamage(DEMON_BONEMARROW_STRENGTH_INCREASE * stats->GetBaseDamage());
	}
}
