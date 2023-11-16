#include "Relics\Offensive\FlameWeapon.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "DamageOverTimeComponent.h"

#define FLAME_WEAPON_DOT_DURATION (1.75f)
#define FLAME_WEAPON_DOT_FRACTION (0.65f)

EntityID FLAME_WEAPON::_OWNER;

const char* FLAME_WEAPON::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you hit an enemy with an attack, *Burn* for an additional %ld%% damage over %.2lf seconds",
		PERCENT(FLAME_WEAPON_DOT_FRACTION),
		FLAME_WEAPON_DOT_DURATION);
	return temp;
}

void FLAME_WEAPON::Initialize(void* input)
{
	// Set the relic's owner
	FLAME_WEAPON::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();
	// Add the DoT to the weapon
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(FLAME_WEAPON::PlaceDamageOverTime);
}

void FLAME_WEAPON::PlaceDamageOverTime(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (FLAME_WEAPON::_OWNER.index != input->attacker.index)
		return;

	StatComponent* attackerStats = registry.GetComponent<StatComponent>(input->attacker);
	StatComponent* defenderStats = registry.GetComponent<StatComponent>(input->defender);

	DamageOverTimeComponent newDoT
	(
		(FLAME_WEAPON_DOT_FRACTION * input->CollapseDamage()) / FLAME_WEAPON_DOT_DURATION,
		FLAME_WEAPON_DOT_DURATION
	);
	DamageOverTimeComponent* EnemyDoT = registry.GetComponent<DamageOverTimeComponent>(input->defender);
	if (nullptr == EnemyDoT || EnemyDoT->LessThan(newDoT))
	{
		registry.AddComponent<DamageOverTimeComponent>(input->defender, newDoT);
	}
}
