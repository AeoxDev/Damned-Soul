#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Relics/DemonHeart.h"
#include "Components.h"
#include "Registry.h"

#define DEMON_HEART_HEALTH_INCREASE 75

EntityID DEMON_HEART::_OWNER;

void DEMON_HEART::Initialize(void* input)
{	
	// Set owner
	DEMON_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(DEMON_HEART::_OWNER)->MarkAsModified();

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
