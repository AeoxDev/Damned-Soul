#include "Relics\Defensive\PainMirror.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"

#define PAIN_MIRROR_RETURN_FRACTION (1.50f)

EntityID PAIN_MIRROR::_OWNER;

const char* PAIN_MIRROR::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you are dealt damage by an enemy attack, that enemy also takes %ld%% of the damage it deals (before applying damage reduction)",
		PERCENT(PAIN_MIRROR_RETURN_FRACTION));
	return temp;
}

void PAIN_MIRROR::Initialize(void* input)
{
	// Set owner
	PAIN_MIRROR::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();


	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(PAIN_MIRROR::Retaliation);
}

void PAIN_MIRROR::Retaliation(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (PAIN_MIRROR::_OWNER.index != input->defender.index)
		return;
	if (input->attacker.index == -1)//Static hazard
	{
		return;
	}
	// The person who's fist hurts a lot
	StatComponent* owMyFistHurts = registry.GetComponent<StatComponent>(input->attacker);

	// The damage
	float damage = ((input->damage + input->flatAdd) * input->incMult) * PAIN_MIRROR_RETURN_FRACTION;

	// Apply the damage
	// Also causes static hazards to flash
	Combat::HitFlat(input->attacker, owMyFistHurts, damage);
}