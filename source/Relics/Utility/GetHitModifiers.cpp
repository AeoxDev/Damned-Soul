#include "Relics/Utility/GetHitModifiers.h"
#include "Relics\RelicFunctions.h"

RelicInput::OnDamageCalculation GetModifiers(EntityID attacker, EntityID defender)
{
	RelicInput::OnDamageCalculation retVal;
	retVal.attacker = attacker;
	retVal.defender = defender;
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_CALC))
		func(&retVal);
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY))
		func(&retVal);
	return retVal;
}

RelicInput::OnDamageCalculation RetaliationCombination(const RelicInput::OnDamageCalculation& original, const RelicInput::OnDamageCalculation& roleReversed, const float fraction)
{
	RelicInput::OnDamageCalculation retVal = roleReversed;
	retVal.damage = original.damage * fraction;
	retVal.flatAdd += original.flatAdd;
	retVal.incMult *= original.incMult;

	return retVal;
}
