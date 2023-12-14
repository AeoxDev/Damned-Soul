#include "Relics\Offensive\LifeSteal.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include <algorithm>

#define LIFE_STEAL_HEALING_MULTIPLIER (.10f)
#define LIFE_STEAL_HEALING_CAP (.55f)
#define LIFE_STEAL_HEALING_SOFT_CAP_LEVEL (35.f)

EntityID LIFE_STEAL::_OWNER;

float _LS_CURRENTLY_HEALED = 0;

const char* LIFE_STEAL::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You heal for %ld%% of the attack damage you deal. This healing can not increase your health above %ld%% of your hit point maximum, and diminishes heavily as you steal more life over the course of a single level", 
		PERCENT(LIFE_STEAL_HEALING_MULTIPLIER),
		PERCENT(LIFE_STEAL_HEALING_CAP));
#pragma warning(suppress : 4172)
	return temp;
}

void LIFE_STEAL::Initialize(void* input)
{
	// Set owner
	LIFE_STEAL::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Level Switch functions
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(LIFE_STEAL::ResetSoftCap);
	// Add it to the list of On Damage Apply functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(LIFE_STEAL::HealFromDamage);
}

void LIFE_STEAL::ResetSoftCap(void* data)
{
 	_LS_CURRENTLY_HEALED = 0.f;
}

void LIFE_STEAL::HealFromDamage(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if this is the owner
	if (input->attacker.index == LIFE_STEAL::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = registry.GetComponent<StatComponent>(_OWNER);

		//float hpMax = stats->GetMaxHealth();
		double healingCap = LIFE_STEAL_HEALING_CAP;
		double healing = LIFE_STEAL_HEALING_MULTIPLIER * input->CollapseDamage();
		double healingFaction = healing / (stats->GetMaxHealth() * (1 + (_LS_CURRENTLY_HEALED / LIFE_STEAL_HEALING_SOFT_CAP_LEVEL)));
		double currentFraction = stats->GetHealthFraction();

		if (currentFraction < healingCap)
		{
			healingFaction -= std::max(0., currentFraction + healingFaction - LIFE_STEAL_HEALING_CAP);
			healing = (float)healingFaction * stats->GetMaxHealth();

			// Heal from the damage dealt, and record the healing
			stats->ApplyHealing(healing);//(LIFE_STEAL_HEALING_MULTIPLIER * input->CollapseDamage());
			_LS_CURRENTLY_HEALED += healing;
		}

	}
}
