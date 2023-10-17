#include "Relics/FlameWeapon.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "DamageOverTimeComponent.h"

#define FLAME_WEAPON_DOT_DURATION (2.f)
#define FLAME_WEAPON_DOT_FRACTION (0.5f)

void FLAME_WEAPON::PlaceDamageOverTime(void* data)
{
	RelicInput::OnHitInput* input = (RelicInput::OnHitInput*)data;

	StatComponent* playerStats = registry.GetComponent<StatComponent>(input->player);
	DamageOverTimeComponent newDoT
	{
		/*Duration*/	FLAME_WEAPON_DOT_DURATION,
		/*DPS*/			(FLAME_WEAPON_DOT_FRACTION * playerStats->damage) / FLAME_WEAPON_DOT_DURATION
	};
	DamageOverTimeComponent* EnemyDoT = registry.GetComponent<DamageOverTimeComponent>(input->enemy);
	if (nullptr == EnemyDoT || EnemyDoT->LessThan(newDoT))
	{
		registry.AddComponent<DamageOverTimeComponent>(input->enemy, newDoT);
	}

}
