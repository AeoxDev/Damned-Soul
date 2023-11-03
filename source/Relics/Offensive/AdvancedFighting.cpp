#include "Relics\Offensive\AdvancedFighting.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"

#define ADVANCED_FIGHTING_CRIT_DAMAGE (2.f)
#define ADVANDED_FIGHTING_SFX_DURATION (0.1f)

EntityID ADVANCED_FIGHTING::_OWNER;

static uint8_t _AF_CurrentCritCharge = 0;

void ADVANCED_FIGHTING::Initialize(void* input)
{
	// Set owner
	ADVANCED_FIGHTING::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	_AF_CurrentCritCharge = 0;

	// Add it to the list of Stat Calc functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(ADVANCED_FIGHTING::CritChance);
}

void _AF_Particles_Begin(EntityID& entity, const int& index)
{
	registry.AddComponent<ParticleComponent>(entity, ADVANDED_FIGHTING_SFX_DURATION, 0, 0.15f, 0, 0, 0, LIGHTNING);
}

void ADVANCED_FIGHTING::CritChance(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (ADVANCED_FIGHTING::_OWNER.index != input->attacker.index)
		return;

	_AF_CurrentCritCharge += 20 + (rand() % 32); // Average of about 36%

	if (100 <= _AF_CurrentCritCharge)
	{

		ParticleAtEntityLocation(input->defender, ADVANDED_FIGHTING_SFX_DURATION, _AF_Particles_Begin);

		input->incMult *= ADVANCED_FIGHTING_CRIT_DAMAGE;
		_AF_CurrentCritCharge -= 100;
	}
}