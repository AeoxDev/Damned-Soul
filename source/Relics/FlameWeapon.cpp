#include "Relics/FlameWeapon.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "DamageOverTimeComponent.h"

#define FLAME_WEAPON_DOT_DURATION (1.75f)
#define FLAME_WEAPON_DOT_FRACTION (0.65f)
#define FLAME_WEAPON_BASE_DAMAGE (0.f)

EntityID FLAME_WEAPON::_OWNER;

void FLAME_WEAPON::Initialize(void* input)
{
	// Set the relic's owner
	FLAME_WEAPON::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();
	// Add the DoT to the weapon
	(*_RelicFunctions)[FUNC_ON_WEAPON_HIT].push_back(FLAME_WEAPON::PlaceDamageOverTime);
}

void FLAME_WEAPON::PlaceDamageOverTime(void* data)
{
	RelicInput::OnHitInput* input = (RelicInput::OnHitInput*)data;

	// Check if it is the right entity that is attacking
	if (FLAME_WEAPON::_OWNER.index != input->attacker.index)
		return;

	StatComponent* playerStats = registry.GetComponent<StatComponent>(input->attacker);
	DamageOverTimeComponent newDoT
	{
		/*Duration*/	FLAME_WEAPON_DOT_DURATION,
		/*DPS*/			(FLAME_WEAPON_BASE_DAMAGE + (FLAME_WEAPON_DOT_FRACTION * playerStats->GetDamage())) / FLAME_WEAPON_DOT_DURATION
	};
	DamageOverTimeComponent* EnemyDoT = registry.GetComponent<DamageOverTimeComponent>(input->defender);
	if (nullptr == EnemyDoT || EnemyDoT->LessThan(newDoT))
	{
		registry.AddComponent<DamageOverTimeComponent>(input->defender, newDoT);
	}

}
