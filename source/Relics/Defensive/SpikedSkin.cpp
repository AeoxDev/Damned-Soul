#include "Relics\Defensive\SpikedSkin.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\GetHitModifiers.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"

#define SPIKED_SKIN_RETURN_FRACTION (.8f)

EntityID SPIKED_SKIN::_OWNER;

const char* SPIKED_SKIN::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you are dealt damage by an enemy attack, that enemy also takes %ld%% of the damage it deals (before applying damage reduction)",
		PERCENT(SPIKED_SKIN_RETURN_FRACTION));
#pragma warning(suppress : 4172)
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
	if (SPIKED_SKIN::_OWNER.index != input->defender.index || input->attacker.index == -1 || input->typeSource & (RelicInput::DMG::REFLECT | RelicInput::DMG::HAZARD))
		return;

	// The person who's fist hurts a lot
	StatComponent* owMyFistHurts = registry.GetComponent<StatComponent>(input->attacker);

	RelicInput::OnDamageCalculation reflected = GetModifiers(input->defender, input->attacker);
	reflected = RetaliationCombination(*input, reflected, SPIKED_SKIN_RETURN_FRACTION);
	reflected.cap = owMyFistHurts->GetHealth();

	DamageNumbers(input->attacker, reflected.CollapseNoCap());

	// Apply the damage
	Combat::HitFlat(input->attacker, owMyFistHurts, reflected.CollapseDamage());
}