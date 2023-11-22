#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "Relics\RelicFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"

bool StatCalcSystem::Update()
{
	auto relevantRelicFuncs = Relics::GetFunctionsOfType(Relics::FUNC_ON_STAT_CALC);

	for (auto entity : View<StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);

		// Act only on modified entities
		if (stats->IsModified())
		{
			stats->ZeroBonusStats();
			// Set input
			RelicInput::OnStatCalcInput input =
			{
				entity,
				stats
			};
			for (auto func : relevantRelicFuncs)
				// Apply relic
				func(&input);

			// It is possible for HP bonus to be reduced to a point that current health is now above maximum without the cap ever being called
			stats->CapHealth();
		}
	}
	return true;
}