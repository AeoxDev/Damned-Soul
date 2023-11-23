#include "Relics\Offensive\DashAttack.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "CombatFunctions.h" //Get 

#define DASH_ATTACK_DAMAGE_MULTIPLIER (0.5f)

EntityID DASH_ATTACK::_OWNER;

const char* DASH_ATTACK::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you dash into or through an enemy, you hit them for %ld%% damage", PERCENT(DASH_ATTACK_DAMAGE_MULTIPLIER));
#pragma warning(suppress : 4172)
	return temp;
}

void DASH_ATTACK::Initialize(void* input)
{
	// Set owner
	DASH_ATTACK::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_DASH].push_back(DASH_ATTACK::DealDamageWhenDashing);
}

void DASH_ATTACK::DealDamageWhenDashing(void* data)
{
	//Build a damage-calculation struct
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (DASH_ATTACK::_OWNER.index != input->attacker.index)
		return;

	//Halve our damage in calculations when we do the dash relic thing
	if (input->attacker.index == DASH_ATTACK::_OWNER.index)
	{
		input->damage *= DASH_ATTACK_DAMAGE_MULTIPLIER;
	}
}
