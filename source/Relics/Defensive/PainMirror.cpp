#include "Relics\Defensive\PainMirror.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\GetHitModifiers.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"

#define PAIN_MIRROR_RETURN_FRACTION (1.1f)

EntityID PAIN_MIRROR::_OWNER;

const char* PAIN_MIRROR::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you are dealt damage by an enemy attack, that enemy also takes %ld%% of the damage it deals (before applying damage reduction)",
		PERCENT(PAIN_MIRROR_RETURN_FRACTION));
#pragma warning(suppress : 4172)
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

	// Check if it is the right entity that is attacking, and if this is reflect damage or a hazard (no infinite loops!)
	if (PAIN_MIRROR::_OWNER.index != input->defender.index || input->attacker.index == -1 || input->typeSource & (RelicInput::DMG::REFLECT | RelicInput::DMG::HAZARD))
		return;

	// The person who's fist hurts a lot
	StatComponent* owMyFistHurts = registry.GetComponent<StatComponent>(input->attacker);

	// The damage modifiers if the defender was the attacker
	RelicInput::OnDamageCalculation reflected = GetModifiers(input->defender, input->attacker);
	reflected = RetaliationCombination(*input, reflected, PAIN_MIRROR_RETURN_FRACTION);
	reflected.cap = owMyFistHurts->GetHealth();


	DamageNumbers(input->attacker, reflected.CollapseNoCap());

	DamageNumbers(input->attacker, damage);

	// Apply the damage
	// Also causes static hazards to flash
	Combat::HitFlat(input->attacker, owMyFistHurts, reflected.CollapseDamage());
}