#include "Relics\Offensive\PowerStrike.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define POWER_STRIKE_BOOST (.35f)

EntityID POWER_STRIKE::_OWNER;

const char* POWER_STRIKE::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases the damage of your charge attaks by up to %ld%%",
		PERCENT(POWER_STRIKE_BOOST));
#pragma warning(suppress : 4172)
	return temp;
}

void POWER_STRIKE::Initialize(void* input)
{
	// Set owner
	POWER_STRIKE::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(POWER_STRIKE::EnhanceChargeAttack);
}

void POWER_STRIKE::EnhanceChargeAttack(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;
	ChargeAttackArgumentComponent* chargeAttack = registry.GetComponent< ChargeAttackArgumentComponent>(input->attacker);

	// Check if this is the owner
	if (input->attacker.index == POWER_STRIKE::_OWNER.index && chargeAttack)
	{
		// Boost damage
		input->incMult *= (1 + (chargeAttack->multiplier - 1) * POWER_STRIKE_BOOST);
	}
}
