#include "Relics\Offensive\DashAttack.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "CombatFunctions.h" //Get 

#define DASH_ATTACK_DAMAGE_MULTIPLIER (0.5f)

EntityID DASH_ATTACK::_OWNER;

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
	//Build a damage-calculation struct (by passing in the stat components of the entities involved?? where does that happen tho)
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (DASH_ATTACK::_OWNER.index != input->attacker.index)
		return;

	//Get attacker stats for the damage we'll be dealing, defender stats for the one who'll get it
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(input->attacker);
	StatComponent* defenderStats = registry.GetComponent<StatComponent>(input->defender);

	//Calculate damage
	float damage = ((input->damage + input->flatAdd) * input->incMult) * DASH_ATTACK_DAMAGE_MULTIPLIER;

	//Apply the damage
	Combat::HitFlat(input->defender, defenderStats, damage);
}
