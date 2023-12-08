#include "Relics\Defensive\BackShield.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "CombatFunctions.h"
#include "DeltaTime.h"

#define BACK_SHIELD_COOLDOWN (2.f)

float _BACK_SHIELD_COOLDOWN = 0.f;

EntityID BACK_SHIELD::_OWNER;

const char* BACK_SHIELD::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You are immune to damage dealt by an attack hitting your back up to once every %.1lf seconds", 
		BACK_SHIELD_COOLDOWN);
#pragma warning(suppress : 4172)
	return temp;
}

void BACK_SHIELD::Initialize(void* input)
{
	// Set owner
	BACK_SHIELD::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// FUlly recharge, just in case
	BACK_SHIELD::FullRecharge(nullptr);

	// Add the functions
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(BACK_SHIELD::FullRecharge);
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(BACK_SHIELD::Recharge);
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(BACK_SHIELD::NullBackHit);
}


void BACK_SHIELD::Recharge(void* data)
{
	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	if (0.0001f < _BACK_SHIELD_COOLDOWN)
	{
		_BACK_SHIELD_COOLDOWN -= input->timeDelta;
		_BACK_SHIELD_COOLDOWN = (0 < _BACK_SHIELD_COOLDOWN) ? _BACK_SHIELD_COOLDOWN : 0;
	}
}


void BACK_SHIELD::FullRecharge(void* data)
{
	// No need for checks
	_BACK_SHIELD_COOLDOWN = 0;
}


void BACK_SHIELD::NullBackHit(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is defending, the shield is off cooldown, and the source is an enemy attack
	if (BACK_SHIELD::_OWNER.index != input->defender.index || 0 < _BACK_SHIELD_COOLDOWN || !(input->typeSource & RelicInput::DMG::INSTANT_ENEMY))
		return;

	auto aTrans = registry.GetComponent<TransformComponent>(input->attacker);
	auto dTrans = registry.GetComponent<TransformComponent>(input->defender);

	float dx, dy, dz;
	dx = dTrans->positionX - aTrans->positionX;
	dy = dTrans->positionY - aTrans->positionY;
	dz = dTrans->positionZ - aTrans->positionZ;

	float dotOfTrans = aTrans->facingX * dx + aTrans->facingY * dy + aTrans->facingZ * dz;

	float dt = GetDeltaTime();

	// If this strike is from behind, nullify the hit and set the cooldown
	// Logically it should be if dot is less than 0, but somewhere things get inverted
	if (0 < dotOfTrans)
	{
		input->decMult = 0;
		_BACK_SHIELD_COOLDOWN = BACK_SHIELD_COOLDOWN;
	}
}