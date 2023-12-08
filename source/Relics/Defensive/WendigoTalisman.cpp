#include "Relics\Defensive\WendigoTalisman.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define WENDIGO_TALISMAN_HEAL_FRACTION (.02f)

EntityID WENDIGO_TALISMAN::_OWNER;
bool _WT_CAN_HEAL = true;

const char* WENDIGO_TALISMAN::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Restores %.1lf%% of your maximum health when you hit an enemy with a fully charged attack",
		WENDIGO_TALISMAN_HEAL_FRACTION * 100.f);
#pragma warning(suppress : 4172)
	return temp;
}

void WENDIGO_TALISMAN::Initialize(void* input)
{
	// Set owner
	WENDIGO_TALISMAN::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(WENDIGO_TALISMAN::HealOnChargeAttack);
	// Add the reset to the functions
	(*_RelicFunctions)[FUNC_ON_NOT_ATTACKING].push_back(WENDIGO_TALISMAN::ResetHeal);
}

void WENDIGO_TALISMAN::ResetHeal(void* input)
{
	_WT_CAN_HEAL = true;
}

void WENDIGO_TALISMAN::HealOnChargeAttack(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;
	ChargeAttackArgumentComponent* chargeAttack = registry.GetComponent< ChargeAttackArgumentComponent>(input->attacker);

	// Check if this is the owner
	if (_WT_CAN_HEAL && input->attacker.index == WENDIGO_TALISMAN::_OWNER.index && chargeAttack && 1.99f <= chargeAttack->multiplier)
	{
		// Lock healing until next attack
		_WT_CAN_HEAL = false;
		// Heal
		StatComponent* stats = registry.GetComponent<StatComponent>(input->attacker);
		stats->ApplyHealing(WENDIGO_TALISMAN_HEAL_FRACTION * stats->GetMaxHealth());
	}
}
