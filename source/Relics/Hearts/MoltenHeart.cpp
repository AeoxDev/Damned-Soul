#include "Relics/Hearts/MoltenHeart.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define MOLTEN_HEART_VALUE 20

EntityID MOLTEN_HEART::_OWNER;

void MOLTEN_HEART::Initialize(void* input)
{
	// Set owner
	MOLTEN_HEART::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	StatComponent* ownerStats = registry.GetComponent<StatComponent>(MOLTEN_HEART::_OWNER);
	ownerStats->MarkAsModified();
	// Molten Heart also heals the when obtained
	ownerStats->UpdateHealth(MOLTEN_HEART_VALUE);



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