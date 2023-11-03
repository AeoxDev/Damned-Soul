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
	defenderStats->UpdateHealth(-1.f * damage, false); // Edit later?

	// Update UI
	RedrawUI();

	// Damage flash
	Combat::DamageFlash(defender, damage);
}

void Combat::HitInteraction(EntityID& attacker, StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats)
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

	// Apply on damage calc functions
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_CALC))
		func(&funcInput);

	// Apply on damage final
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY))
		func(&funcInput);

	float finalDamage = funcInput.CollapseDamage();

	// Provide a flat hit, mostly just so that we can edit all sources at the same time
	Combat::HitFlat(defender, defenderStats, finalDamage);
}