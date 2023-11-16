#include "Relics\Defensive\Hearts\CorruptedHeart.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define CORRUPTED_HEART_HEALTH_INCREASE (40)

EntityID CORRUPTED_HEART::_OWNER;

const char* CORRUPTED_HEART::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your health by %ld",
		CORRUPTED_HEART_HEALTH_INCREASE);
	return temp;
}

void CORRUPTED_HEART::Initialize(void* input)
{
	// Set owner
	CORRUPTED_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	// It also raises max HP while elevating current hp to match, meaning this is nessecary
	RELIC_RAISE_CURRENT_MAX_HP(CORRUPTED_HEART::_OWNER, CORRUPTED_HEART_HEALTH_INCREASE);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(CORRUPTED_HEART::IncreaseHealth);
}

void CORRUPTED_HEART::IncreaseHealth(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == CORRUPTED_HEART::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusHealth(+CORRUPTED_HEART_HEALTH_INCREASE);
	}
}