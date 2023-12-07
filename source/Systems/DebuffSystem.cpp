#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "CombatFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"

bool DebuffSystem::Update()
{
	for (auto entity : View<DebuffComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(entity);

		// Try to fetch a model component
		ModelSkeletonComponent* characterModel = registry.GetComponent<ModelSkeletonComponent>(entity);
		if (nullptr == characterModel)
		{
			// Try to fetch a boneless model if a boned one doesn't exist, shared data is in the same place for both, so it should work
			characterModel = (ModelSkeletonComponent*)registry.GetComponent<ModelBonelessComponent>(entity);
		}

		for (uint8_t i = 0; i < DamageOverTime::END_OF_DOTS; ++i)
		{
			if (debuff->m_dots[i])
			{
				// Calculate effectiv DPS
				// There is no real source entity in this case
				float effectiveDPS = Combat::CalculateDamage(debuff->m_dots[i], entity, RelicInput::DMG::DOT);

				// Get the remaining time of the dot
				float remainingTime = debuff->m_dots[i].GetTime();

				// Flat hit using the damage and time calculated
				if (registry.GetComponent<PlayerComponent>(entity) != nullptr)
				{
					Combat::HitFlat(entity, stats, remainingTime * effectiveDPS, remainingTime, true);
				}
				else
				{
					Combat::HitFlat(entity, stats, remainingTime * effectiveDPS);
				}

				debuff->m_dots[i].AlterModelColor(characterModel, DamageOverTime::DOT_TYPE(i));
			}
		}

		if (debuff->m_frozen && characterModel)
		{
				float ft = std::min(debuff->m_frozen.GetTime() * debuff->m_frozen.GetTime(), 1.f);
				// Multiplicative
				characterModel->shared.bcmR_temp /= (1 + (.05f + ft));
				characterModel->shared.bcmG_temp /= (1 + (.05f + ft));
				characterModel->shared.bcmB_temp /= (1 + (.05f + ft));
				// Additive
				characterModel->shared.bcaR_temp += .125f + 0.25f * ft;
				characterModel->shared.bcaG_temp += .125f + 0.3f * ft;
				characterModel->shared.bcaB_temp += .2f + 0.4f * ft;
		}


		// Count down the Debuffs' time and remove them if they are done
		if (debuff->Advance())
			registry.RemoveComponent<DebuffComponent>(entity);
	}

	return true;
}