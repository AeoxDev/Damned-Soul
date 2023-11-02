#include "Relics/Hearts/CorruptedHeart.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define CORRUPTED_HEART_HEALTH_INCREASE 40

EntityID CORRUPTED_HEART::_OWNER;

void CORRUPTED_HEART::Initialize(void* input)
{
	// Set owner
	CORRUPTED_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(CORRUPTED_HEART::_OWNER)->MarkAsModified();

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