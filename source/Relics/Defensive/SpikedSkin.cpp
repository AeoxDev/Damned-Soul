#include "Relics\Defensive\SpikedSkin.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"

#define SPIKED_SKIN_RETURN_FRACTION (1.f)

EntityID SPIKED_SKIN::_OWNER;

const char* SPIKED_SKIN::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf(temp, "Whenever you are dealt damage by an enemy attack, that enemy also takes %ld%% of the damage it deals (before applying damage reduction)",
		PERCENT(SPIKED_SKIN_RETURN_FRACTION));
	return temp;
}

void SPIKED_SKIN::Initialize(void* input)
{
	// Set owner
	SPIKED_SKIN::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();


	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(SPIKED_SKIN::Retaliation);
}

void SPIKED_SKIN::Retaliation(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (SPIKED_SKIN::_OWNER.index != input->defender.index || input->attacker.index < 0)
		return;

	// The person who's fist hurts a lot
	StatComponent* owMyFistHurts = registry.GetComponent<StatComponent>(input->attacker);

	// The damage
	float damage = ((input->damage + input->flatAdd) * input->incMult) * SPIKED_SKIN_RETURN_FRACTION;

	// Apply the damage
	// Also causes static hazards to flash
	Combat::HitFlat(input->attacker, owMyFistHurts, damage);
}