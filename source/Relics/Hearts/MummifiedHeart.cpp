#include "Relics/Hearts/MummifiedHeart.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define MUMMIFIED_HEART_HEALTH_INCREASE 15

EntityID MUMMIFIED_HEART::_OWNER;

void MUMMIFIED_HEART::Initialize(void* input)
{
	// Set owner
	MUMMIFIED_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(MUMMIFIED_HEART::_OWNER)->MarkAsModified();

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