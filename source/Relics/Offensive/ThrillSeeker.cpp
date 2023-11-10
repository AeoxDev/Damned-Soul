#include "Relics\Offensive\ThrillSeeker.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

EntityID THRILL_SEEKER::_OWNER;

void THRILL_SEEKER::Initialize(void* input)
{
	// Set owner
	THRILL_SEEKER::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(THRILL_SEEKER::DamageAmplifier);
}

void THRILL_SEEKER::DamageAmplifier(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if the owner of this relic is attacking someone
	if (input->attacker.index == THRILL_SEEKER::_OWNER.index)
	{
		// Increase the damage dealt in relation to health lost
		StatComponent* attackerStats = registry.GetComponent<StatComponent>(input->attacker);
		input->incMult *= 1.0f + (1.0f - attackerStats->GetHealthFraction());
	}
}