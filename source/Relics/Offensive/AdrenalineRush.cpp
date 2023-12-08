#include "Relics\Offensive\AdrenalineRush.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define ADRENALINE_RUSH_ACTIVE_DURATION 3.f
#define ADRENALINE_RUSH_ATTACK_SPEED_INCREASE 1.0f
float _AR_REMAINING_TIME = 0.f;

EntityID ADRENALINE_RUSH::_OWNER;

const char* ADRENALINE_RUSH::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You attack %ld%% faster after taking damage, fading over %.1lf seconds",
		PERCENT(ADRENALINE_RUSH_ATTACK_SPEED_INCREASE),
		ADRENALINE_RUSH_ACTIVE_DURATION);
#pragma warning(suppress : 4172)
	return temp;
}

void ADRENALINE_RUSH::Initialize(void* input)
{
	// Set owner
	ADRENALINE_RUSH::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(ADRENALINE_RUSH::_OWNER)->MarkAsModified();

	_AR_REMAINING_TIME = 0.f;

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_DAMAGE_TAKEN].push_back(ADRENALINE_RUSH::OnDamageTaken);
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(ADRENALINE_RUSH::OnUpdate);
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(ADRENALINE_RUSH::AttackSpeedBoost);
}

void ADRENALINE_RUSH::OnDamageTaken(void* data)
{
	RelicInput::OnHealthUpdate* input = (RelicInput::OnHealthUpdate*)data;

	if (input->adressOfStatComponent == registry.GetComponent<StatComponent>(ADRENALINE_RUSH::_OWNER))
	{
		_AR_REMAINING_TIME = ADRENALINE_RUSH_ACTIVE_DURATION;
	}
}

void ADRENALINE_RUSH::OnUpdate(void* data)
{
	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	_AR_REMAINING_TIME -= input->timeDelta;
	_AR_REMAINING_TIME = (0 < _AR_REMAINING_TIME) * _AR_REMAINING_TIME;
}

void ADRENALINE_RUSH::AttackSpeedBoost(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == ADRENALINE_RUSH::_OWNER.index && _AR_REMAINING_TIME)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		//Double attack speed (regularly it's 1, so we make it 2)
		stats->UpdateBonusAttackSpeed(+ADRENALINE_RUSH_ATTACK_SPEED_INCREASE * _AR_REMAINING_TIME / ADRENALINE_RUSH_ACTIVE_DURATION);
	}
}