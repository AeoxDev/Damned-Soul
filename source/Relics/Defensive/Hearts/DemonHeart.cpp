#include "Relics\Defensive\Hearts\DemonHeart.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_HEART_HEALTH_INCREASE 25

EntityID DEMON_HEART::_OWNER;

const char* DEMON_HEART::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your health by %ld",
		DEMON_HEART_HEALTH_INCREASE);
	return temp;
}

void DEMON_HEART::Initialize(void* input)
{	
	// Set owner
	DEMON_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	// It also raises max HP while elevating current hp to match, meaning this is nessecary
	RELIC_RAISE_CURRENT_MAX_HP(DEMON_HEART::_OWNER, DEMON_HEART_HEALTH_INCREASE);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(DEMON_HEART::IncreaseHealth);
}

void DEMON_HEART::IncreaseHealth(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == DEMON_HEART::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusHealth(+DEMON_HEART_HEALTH_INCREASE);
	}
}