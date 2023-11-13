#include "Relics\Defensive\PainMirror.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"

#define PAIN_MIRROR_RETURN_FRACTION (0.30f)
#define PAIN_MIRROR_RETURN_FLAT (0.f)

EntityID PAIN_MIRROR::_OWNER;

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

	// The person who's fist hurts a lot
	StatComponent* owMyFistHurts = registry.GetComponent<StatComponent>(input->attacker);

	// The damage
	float damage = ((input->damage + input->flatAdd) * input->incMult) * PAIN_MIRROR_RETURN_FRACTION + PAIN_MIRROR_RETURN_FLAT;

	// Apply the damage
	// Also causes static hazards to flash
	Combat::HitFlat(input->attacker, owMyFistHurts, damage);
}