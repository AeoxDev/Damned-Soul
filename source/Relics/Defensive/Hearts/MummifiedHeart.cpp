#include "Relics\Defensive\Hearts\MummifiedHeart.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define MUMMIFIED_HEART_HEALTH_INCREASE 15

EntityID MUMMIFIED_HEART::_OWNER;

const char* MUMMIFIED_HEART::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your health by %ld",
		MUMMIFIED_HEART_HEALTH_INCREASE);
#pragma warning(suppress : 4172)
	return temp;
}

void MUMMIFIED_HEART::Initialize(void* input)
{
	// Set owner
	MUMMIFIED_HEART::_OWNER = *((EntityID*)input);

	registry.GetComponent<StatComponent>(MUMMIFIED_HEART::_OWNER)->MarkAsModified();
	//// This is a stat altering relic, mark the entity as having modified stats
	//// It also raises max HP while elevating current hp to match, meaning this is nessecary
	//RELIC_RAISE_CURRENT_MAX_HP(MUMMIFIED_HEART::_OWNER, MUMMIFIED_HEART_HEALTH_INCREASE);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(MUMMIFIED_HEART::IncreaseHealth);
}

void MUMMIFIED_HEART::IncreaseHealth(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == MUMMIFIED_HEART::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusHealth(+MUMMIFIED_HEART_HEALTH_INCREASE);
	}
}