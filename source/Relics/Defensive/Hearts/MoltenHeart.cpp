#include "Relics\Defensive\Hearts\MoltenHeart.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define MOLTEN_HEART_VALUE 20

EntityID MOLTEN_HEART::_OWNER;

void MOLTEN_HEART::Initialize(void* input)
{
	// Set owner
	MOLTEN_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	// It also raises max HP while elevating current hp to match, meaning this is nessecary
	RELIC_RAISE_CURRENT_MAX_HP(MOLTEN_HEART::_OWNER, MOLTEN_HEART_VALUE);

	// Molten Heart also heals (real healing, not just HP elevation to match max hp increase) the when obtained
	registry.GetComponent<StatComponent>(MOLTEN_HEART::_OWNER)->ApplyHealing(MOLTEN_HEART_VALUE);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(MOLTEN_HEART::IncreaseHealth);
}

void MOLTEN_HEART::IncreaseHealth(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == MOLTEN_HEART::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusHealth(+MOLTEN_HEART_VALUE);
	}
}