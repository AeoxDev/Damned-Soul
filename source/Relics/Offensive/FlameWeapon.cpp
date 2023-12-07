#include "Relics\Offensive\FlameWeapon.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define FLAME_WEAPON_DOT_DURATION (2.f)
#define FLAME_WEAPON_DOT_FRACTION (0.5f)

EntityID FLAME_WEAPON::_OWNER;

const char* FLAME_WEAPON::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you hit an enemy with an attack, *Burn* for an additional %ld%% damage over %.2lf seconds",
		PERCENT(FLAME_WEAPON_DOT_FRACTION),
		FLAME_WEAPON_DOT_DURATION);
#pragma warning(suppress : 4172)
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

	DamageOverTime newDoT
	(
		(FLAME_WEAPON_DOT_FRACTION * input->CollapseDamage()) / FLAME_WEAPON_DOT_DURATION,
		FLAME_WEAPON_DOT_DURATION
	);
	DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(input->defender);
	if (nullptr == debuff)
	{
		registry.AddComponent<DebuffComponent>(input->defender, DamageOverTime::BURN, newDoT);
		registry.AddComponent<ParticleComponent>(input->defender, FLAME_WEAPON_DOT_DURATION, 100.f, 1.0f, 0.0f, 5.0f, 0.0f, 3, ComputeShaders::ON_FIRE);
	}
	else if (debuff->m_dots[DamageOverTime::BURN].LessThan(newDoT))
	{
		debuff->m_dots[DamageOverTime::BURN] = newDoT;
	}
}
