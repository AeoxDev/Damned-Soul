#include "Relics\Defensive\LastStand.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"
#include "DeltaTime.h"

#define LAST_STAND_THRESHOLD (.5f)
#define LAST_STAND_MULTIPLIER (.5f)
#define LAST_STAND_COOLDOWN (2.f)

float _LAST_STAND_COOLDOWN = 0.f;

EntityID LAST_STAND::_OWNER;

void LAST_STAND::Initialize(void* input)
{
	// Set owner
	LAST_STAND::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// FUlly recharge, just in case
	LAST_STAND::FullRecharge(nullptr);

	// Add the functions
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(LAST_STAND::FullRecharge);
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(LAST_STAND::Recharge);
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(LAST_STAND::ResistDamage);
}


void LAST_STAND::Recharge(void* data)
{
	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	if (0.0001f < _LAST_STAND_COOLDOWN)
	{
		_LAST_STAND_COOLDOWN -= input->timeDelta;
		_LAST_STAND_COOLDOWN = (0 < _LAST_STAND_COOLDOWN) ? _LAST_STAND_COOLDOWN : 0;
	}
}


void LAST_STAND::FullRecharge(void* data)
{
	// No need for checks
	_LAST_STAND_COOLDOWN = 0;
}


void LAST_STAND::ResistDamage(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is defending, the shield is off cooldown, and the source is an enemy attack
	if (LAST_STAND::_OWNER.index != input->defender.index || 0 < _LAST_STAND_COOLDOWN || !(input->typeSource & RelicInput::DMG::INSTANT_ENEMY))
		return;

	float hpFraction = registry.GetComponent<StatComponent>(LAST_STAND::_OWNER)->GetHealthFraction();

	// If this strike happens while below 50% health, reduce damage by half and put on cooldown
	if (hpFraction <= LAST_STAND_THRESHOLD)
	{
		input->decMult = LAST_STAND_MULTIPLIER;
		_LAST_STAND_COOLDOWN = LAST_STAND_COOLDOWN;
	}
}