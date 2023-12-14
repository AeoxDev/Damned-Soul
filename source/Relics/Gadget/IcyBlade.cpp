#include "Relics\Gadget\IcyBlade.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define ICY_BLADE_FREEZE_CHANCE (0.175f)
#define ICY_BLADE_DURATION (2.f)

EntityID ICY_BLADE::_OWNER;

const char* ICY_BLADE::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You gain %.1lf%% chance to freeze enemies for %.1lf seconds on a hit", 
		ICY_BLADE_FREEZE_CHANCE * 100.f,
		ICY_BLADE_DURATION);
#pragma warning(suppress : 4172)
	return temp;
}

void ICY_BLADE::Initialize(void* input)
{
	// Set owner
	ICY_BLADE::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add the modify speed function to the stat calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(ICY_BLADE::FreezeOnHit);
}

void ICY_BLADE::FreezeOnHit(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner and RNGesus allows the freeze
	if (input->attacker.index == ICY_BLADE::_OWNER.index && (rand() % 100) < PERCENT(ICY_BLADE_FREEZE_CHANCE))
	{
		// Freeze them!
		DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(input->defender);
		if (debuff && debuff->m_frozen.LessThan(ICY_BLADE_DURATION))
		{
			debuff->m_frozen = Frozen(ICY_BLADE_DURATION);
		}
		else if (nullptr == debuff)
		{
			registry.AddComponent<DebuffComponent>(input->defender, Frozen(ICY_BLADE_DURATION));
		}
	}
}