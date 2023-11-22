#include "CombatFunctions.h"
#include "EventFunctions.h"
#include "Relics\RelicFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "UIRenderer.h"

#define FLASH_TIME(dmg) ((dmg * 0.025f) + 0.2f)

void Combat::DamageFlash(EntityID& defender, const float damage)
{
	AddTimedEventComponentStartContinuousEnd(defender, 0.f, nullptr, BlinkColor, FLASH_TIME(damage), ResetColor);
}

void Combat::HitFlat(EntityID& defender, StatComponent* defenderStats, const float damage)
{
	// Update health
	defenderStats->ApplyDamage(damage, false); // Edit later?

	//Play sound when hit by hazard
	AddTimedEventComponentStartEnd(defender, 0.0f, HurtSound, 0.25f, nullptr, 0, 1);

	// Update UI
	RedrawUI();

	// Damage flash
	Combat::DamageFlash(defender, damage);

	// TODO: Play ANIMATION_TAKE_DAMAGE. Timed event?
}

float Combat::CalculateDamage(const DamageOverTime& dot, EntityID& defender, const uint64_t& source)
{
	RelicInput::OnDamageCalculation funcInput;
	funcInput.defender = defender;
	funcInput.damage = dot.GetDPS();
	funcInput.cap = 99999999; // No real cap for DPS
	funcInput.typeSource = RelicInput::DMG::DAMAGE_TYPE_AND_SOURCE(source);

	// Apply on damage calc functions
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_CALC))
		func(&funcInput);

	// Apply on damage final
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY))
		func(&funcInput);

	return funcInput.CollapseDamage();
}

float Combat::CalculateDamage(const EntityID& attacker, const StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats, const uint64_t& source)
{
	RelicInput::OnDamageCalculation funcInput;
	funcInput.attacker = attacker;
	funcInput.defender = defender;
	funcInput.damage = attackerStats->GetDamage();
	funcInput.cap = defenderStats->GetHealth();
	funcInput.typeSource = RelicInput::DMG::DAMAGE_TYPE_AND_SOURCE(source);

	// Increase if charge attack
	ChargeAttackArgumentComponent* charge = registry.GetComponent<ChargeAttackArgumentComponent>(attacker);
	if (charge)
	{
		funcInput.incMult *= charge->multiplier;
	}

	// Apply on damage calc functions
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_CALC))
		func(&funcInput);

	// Apply on damage final
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY))
		func(&funcInput);

	return funcInput.CollapseDamage();
}

void Combat::HitInteraction(const EntityID& attacker, const StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats/*, bool isCharged*/)
{
	if (attackerStats == nullptr || defenderStats == nullptr)
	{
		return;
	}
	// Calculate damage
	float finalDamage = CalculateDamage(attacker, attackerStats, defender, defenderStats, RelicInput::DMG::INSTANT_ENEMY);

	// Provide a flat hit, mostly just so that we can edit all sources at the same time
	Combat::HitFlat(defender, defenderStats, finalDamage);
}

void Combat::DashHitInteraction(EntityID& attacker, StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats)
{
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(defender);
	//Deal regular damage as well as on-hit damage from potential relics

	// Calculate damage
	// Some values can start with default
	RelicInput::OnDamageCalculation funcInput;
	funcInput.attacker = attacker;
	funcInput.defender = defender;
	funcInput.damage = attackerStats->GetDamage();
	funcInput.cap = defenderStats->GetHealth();

	//Halve the damage since we're dashing
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DASH))
		func(&funcInput);

	//Calculate damage modifications from relics
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_CALC))
		func(&funcInput);

	//Calculate things that happen when damage is being applied (Reflect damage, lifesteal, etc..)
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY))
		func(&funcInput);

	float finalDamage = funcInput.CollapseDamage();

	// Provide a flat hit, mostly just so that we can edit all sources at the same time
	Combat::HitFlat(defender, defenderStats, finalDamage);
}