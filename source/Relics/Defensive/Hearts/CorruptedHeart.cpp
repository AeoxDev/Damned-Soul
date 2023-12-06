#include "Relics\Defensive\Hearts\CorruptedHeart.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define CORRUPTED_HEART_HEALTH_INCREASE (25)
#define CORRUPTED_HEART_DAMAGE_CONVERT_MOD (.65f)
#define CORRUPTED_HEART_DURATION (3.f)

EntityID CORRUPTED_HEART::_OWNER;

const char* CORRUPTED_HEART::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Increases your health by %ld and converts %ld%% of the attack damage you take into a damage over time effect. This damage is dealt over %.1lf seconds, does not stack, and the damage you take each second is reduced by your damage reducing effects",
		CORRUPTED_HEART_HEALTH_INCREASE,
		100 - PERCENT(CORRUPTED_HEART_DAMAGE_CONVERT_MOD),
		CORRUPTED_HEART_DURATION);
#pragma warning(suppress : 4172)
	return temp;
}

void CORRUPTED_HEART::Initialize(void* input)
{
	// Set owner
	CORRUPTED_HEART::_OWNER = *((EntityID*)input);

	registry.GetComponent<StatComponent>(CORRUPTED_HEART::_OWNER)->MarkAsModified();
	//// This is a stat altering relic, mark the entity as having modified stats
	//// It also raises max HP while elevating current hp to match, meaning this is nessecary
	//RELIC_RAISE_CURRENT_MAX_HP(CORRUPTED_HEART::_OWNER, CORRUPTED_HEART_HEALTH_INCREASE);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Reduce instant damage ...
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(CORRUPTED_HEART::DamageReduction);

	// ... and take it over time instead
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(CORRUPTED_HEART::ApplyDot);

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(CORRUPTED_HEART::IncreaseHealth);
}

void CORRUPTED_HEART::DamageReduction(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// If the damage is instant, reduce it
	if ((input->defender.index == CORRUPTED_HEART::_OWNER.index) && (RelicInput::DMG::INSTANT & input->typeSource))
	{
		input->decMult *= CORRUPTED_HEART_DAMAGE_CONVERT_MOD;
	}
}

void CORRUPTED_HEART::ApplyDot(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// If instant damage was taken, potentially apply a new dot equal to the portion shipped off
	if ((input->defender.index == CORRUPTED_HEART::_OWNER.index) && (RelicInput::DMG::INSTANT & input->typeSource))
	{
		// The damage over time to apply
		DamageOverTime newDoT
		(
			((1.f - CORRUPTED_HEART_DAMAGE_CONVERT_MOD) * input->damage) / CORRUPTED_HEART_DURATION,
			CORRUPTED_HEART_DURATION
		);

		DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(input->defender);
		if (nullptr == debuff)
		{
			registry.AddComponent<DebuffComponent>(input->defender, DamageOverTime::POISON, newDoT);
		}
		else if (debuff->m_dots[DamageOverTime::POISON].LessThan(newDoT))
		{
			debuff->m_dots[DamageOverTime::POISON] = newDoT;
		}
	}
}

void CORRUPTED_HEART::IncreaseHealth(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == CORRUPTED_HEART::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		// Increase damage
		stats->UpdateBonusHealth(+CORRUPTED_HEART_HEALTH_INCREASE);
	}
}