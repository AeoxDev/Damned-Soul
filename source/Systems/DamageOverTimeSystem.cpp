#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "CombatFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"

bool DebuffSystem::Update()
{
	// Won't update UI if the player is being DoT:ed

	for (auto entity : View<DebuffComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(entity);

		if (debuff->m_dot)
		{
			// Calculate effectiv DPS
			// There is no real source entity in this case
			float effectiveDPS = Combat::CalculateDamage(debuff->m_dot, entity, RelicInput::DMG::DOT);

			// Get the remaining time of the dot
			float remainingTime = debuff->m_dot.GetTime();

			// Flat hit using the damage and time calculated
			Combat::HitFlat(entity, stats, remainingTime * effectiveDPS);
		}
		//if (debuff->m_frozen)
		//{
		//	// This shit happens in behaviors, not in the debuff system
		//}


		// Count down the Debuffs' time and remove them if they are done
		if (debuff->Advance())
			registry.RemoveComponent<DebuffComponent>(entity);
	}

	return true;
}