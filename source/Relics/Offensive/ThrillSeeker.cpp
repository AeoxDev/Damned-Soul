#include "Relics\Offensive\ThrillSeeker.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

EntityID THRILL_SEEKER::_OWNER;

#define THRILL_SEEKER_RATIO (.75f)

const char* THRILL_SEEKER::Description()
{
	float bonus, per;
	if (1.f <= THRILL_SEEKER_RATIO)
	{
		per = 1.f;
		bonus = THRILL_SEEKER_RATIO;
	}
	else
	{
		bonus = 1.f;
		per = 1.f / THRILL_SEEKER_RATIO;
	}

	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You deal %.1lf%% more damage for every %.1lf%% health you are missing",
		bonus,
		per);
#pragma warning(suppress : 4172)
	return temp;
}

void THRILL_SEEKER::Initialize(void* input)
{
	// Set owner
	THRILL_SEEKER::_OWNER = *((EntityID*)input);

	registry.GetComponent<StatComponent>(THRILL_SEEKER::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(THRILL_SEEKER::DamageAmplifier);
	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(THRILL_SEEKER::DamageAmplifier);
}

void THRILL_SEEKER::DamageAmplifier(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if the owner of this relic is attacking someone
	if (input->attacker.index == THRILL_SEEKER::_OWNER.index)
	{
		// Increase the damage dealt in relation to health lost
		StatComponent* attackerStats = registry.GetComponent<StatComponent>(input->attacker);
		input->incMult *= 1.0f + THRILL_SEEKER_RATIO * (1.0f - attackerStats->GetHealthFraction());
	}
}