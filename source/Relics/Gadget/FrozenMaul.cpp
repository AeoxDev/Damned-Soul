#include "Relics\Gadget\FrozenMaul.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define FROZEN_MAUL_FREEZE_CHANCE (0.125f)
#define FROZEN_MAUL_DURATION (1.5f)
#define FROZEN_MAUL_KNOCKBACK_INCREASE (0.35f)

EntityID FROZEN_MAUL::_OWNER;

const char* FROZEN_MAUL::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Your knockback force is increased by %ld%% and you gain %.1lf%% chance to freeze enemies for %.1lf seconds on a hit",
		PERCENT(FROZEN_MAUL_KNOCKBACK_INCREASE),
		FROZEN_MAUL_FREEZE_CHANCE * 100.f,
		FROZEN_MAUL_DURATION);
#pragma warning(suppress : 4172)
	return temp;
}

void FROZEN_MAUL::Initialize(void* input)
{
	// Set owner
	FROZEN_MAUL::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(FROZEN_MAUL::IncreaseKnockback);

	// Add the modify speed function to the stat calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(FROZEN_MAUL::FreezeOnHit);
}

void FROZEN_MAUL::IncreaseKnockback(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == FROZEN_MAUL::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusKnockback(FROZEN_MAUL_KNOCKBACK_INCREASE);
	}
}

void FROZEN_MAUL::FreezeOnHit(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner and RNGesus allows the freeze
	if (input->attacker.index == FROZEN_MAUL::_OWNER.index && (rand() % 100) < PERCENT(FROZEN_MAUL_FREEZE_CHANCE))
	{
		// Freeze them!
		DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(input->defender);
		if (debuff && debuff->m_frozen.LessThan(FROZEN_MAUL_DURATION))
		{
			debuff->m_frozen = Frozen(FROZEN_MAUL_DURATION);
		}
		else if (nullptr == debuff)
		{
			registry.AddComponent<DebuffComponent>(input->defender, Frozen(FROZEN_MAUL_DURATION));
		}
	}
}