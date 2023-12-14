#include "Relics\Defensive\GoldenDuck.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"
#include "DeltaTime.h"
#include "States\StateManager.h"

#define GOLDEN_DUCK_BASE_DURATION (3.f)
#define GOLDEN_DUCK_BASE_CD (0.25f)
#define GOLDEN_DUCK_RECHARGE_RATE (0.5f)

float _GOLDEN_DUCK_REMAINING_TIME = GOLDEN_DUCK_BASE_DURATION;
float _GOLDEN_DUCK_RECHARGE_COOLDOWN = 0.f;

EntityID GOLDEN_DUCK::_OWNER;

const char* GOLDEN_DUCK::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You can stand in damaging hazards for up to %.1lf seconds without taking damage. Rechargers over %.1lf seconds while not in use",
		GOLDEN_DUCK_BASE_DURATION,
		GOLDEN_DUCK_BASE_DURATION / GOLDEN_DUCK_RECHARGE_RATE);
#pragma warning(suppress : 4172)
	return temp;
}

void GOLDEN_DUCK::Initialize(void* input)
{
	// Set owner
	GOLDEN_DUCK::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// FUlly recharge, just in case
	GOLDEN_DUCK::FullRecharge(nullptr);

	// Add the functions
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(GOLDEN_DUCK::FullRecharge);
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(GOLDEN_DUCK::Recharge);
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(GOLDEN_DUCK::NullHazardDot);
}


void GOLDEN_DUCK::Recharge(void* data)
{
	if (currentStates & InPause)
		return;

	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	if (0.0001f < _GOLDEN_DUCK_RECHARGE_COOLDOWN)
	{
		_GOLDEN_DUCK_RECHARGE_COOLDOWN -= input->timeDelta;
	}
	else if (_GOLDEN_DUCK_RECHARGE_COOLDOWN < GOLDEN_DUCK_BASE_DURATION)
	{
		// Recharge at a given rate
		_GOLDEN_DUCK_REMAINING_TIME += input->timeDelta * GOLDEN_DUCK_RECHARGE_RATE;
		// Branchless time cap
		_GOLDEN_DUCK_REMAINING_TIME = (_GOLDEN_DUCK_REMAINING_TIME < GOLDEN_DUCK_BASE_DURATION) ? _GOLDEN_DUCK_REMAINING_TIME : GOLDEN_DUCK_BASE_DURATION;
	}
}


void GOLDEN_DUCK::FullRecharge(void* data)
{
	// No need for checks
	_GOLDEN_DUCK_RECHARGE_COOLDOWN = GOLDEN_DUCK_BASE_CD;
	_GOLDEN_DUCK_REMAINING_TIME = GOLDEN_DUCK_BASE_DURATION;
}


void GOLDEN_DUCK::NullHazardDot(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// The golden duck cannot recharge for a short while
	_GOLDEN_DUCK_RECHARGE_COOLDOWN = GOLDEN_DUCK_BASE_CD;

	// Check if it is the right entity that is defending, the golden duck has time remaining, and the source of the damage counts as a dot from a hazard
	if (GOLDEN_DUCK::_OWNER.index != input->defender.index || _GOLDEN_DUCK_REMAINING_TIME < 0.0001f || !(input->typeSource & RelicInput::DMG::DOT_HAZARD))
		return;

	float dt = GetDeltaTime();

	if (dt < _GOLDEN_DUCK_REMAINING_TIME)
	{
		// If the golden duck has enough time to completely remove the damage, do so
		input->decMult = 0;
		_GOLDEN_DUCK_REMAINING_TIME -= dt;
	}
	else
	{	// Otherwise, remove a fraction
		input->decMult *= _GOLDEN_DUCK_REMAINING_TIME / dt;
		_GOLDEN_DUCK_REMAINING_TIME = 0;
	}
}