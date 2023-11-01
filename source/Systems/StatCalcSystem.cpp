#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "Relics\RelicFunctions.h"
#include "Relics\RelicFuncInputTypes.h"

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
			for (auto func : relevantRelicFuncs)
			{
				// Set input
				RelicInput::OnStatCalcInput input =
				{
					entity,
					stats
				};
				// Apply relic
				func(&input);
			}
		}
	}
	return true;
}