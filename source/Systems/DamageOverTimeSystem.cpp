#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "CombatFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"

bool DamageOverTimeSystem::Update()
{
	// Won't update UI if the player is being DoT:ed

	for (auto entity : View<DamageOverTimeComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		DamageOverTimeComponent* DoT = registry.GetComponent<DamageOverTimeComponent>(entity);

		// Calculate effectiv DPS
		// There is no real source entity in this case
		float effectiveDPS = Combat::CalculateDamage(DoT, entity, RelicInput::DMG::DOT);

		// Get the remaining time of the dot
		float remainingTime = DoT->GetTime();

		// Flat hit using the damage and time calculated
		Combat::HitFlat(entity, stats, remainingTime * effectiveDPS);

		// Count down the DoT's time and remove it if it is done
		if (DoT->Advance())
			registry.RemoveComponent<DamageOverTimeComponent>(entity);
	}

	return true;
}