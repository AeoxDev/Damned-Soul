#include "Relics/FlameWeapon.h"
#include "Relics/RelicInternalHelper.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "DamageOverTimeComponent.h"

#define FLAME_WEAPON_DOT_DURATION (1.75f)
#define FLAME_WEAPON_DOT_FRACTION (0.65f)
#define FLAME_WEAPON_BASE_DAMAGE (0.f)

void FLAME_WEAPON::Initialize(void* input)
{
	// Make sure the relic function map exists
	_validateRelicFunctions();
	// Add the DoT to the weapon
	(*_RelicFunctions)[FUNC_ON_WEAPON_HIT].push_back(FLAME_WEAPON::PlaceDamageOverTime);
}

void FLAME_WEAPON::PlaceDamageOverTime(void* data)
{
	RelicInput::OnHitInput* input = (RelicInput::OnHitInput*)data;

	StatComponent* playerStats = registry.GetComponent<StatComponent>(input->player);
	DamageOverTimeComponent newDoT
	{
		/*Duration*/	FLAME_WEAPON_DOT_DURATION,
		/*DPS*/			(FLAME_WEAPON_BASE_DAMAGE + (FLAME_WEAPON_DOT_FRACTION * playerStats->damage)) / FLAME_WEAPON_DOT_DURATION
	};
	DamageOverTimeComponent* EnemyDoT = registry.GetComponent<DamageOverTimeComponent>(input->enemy);
	if (nullptr == EnemyDoT || EnemyDoT->LessThan(newDoT))
	{
		registry.AddComponent<DamageOverTimeComponent>(input->enemy, newDoT);
	}

}
