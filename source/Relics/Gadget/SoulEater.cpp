#include "Relics\Gadget\SoulEater.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\SoulRelicHelper.h"
#include "Components.h"
#include "Registry.h"
#include <cmath>

#define SOUL_EATER_HEAL_RATIO_GAIN (1.0f/1.f)
#define SOUL_EATER_HEAL_RATIO_LOSE (2.0f/1.f)

EntityID SOUL_EATER::_OWNER;

const char* SOUL_EATER::Description()
{
	float bonus, per;
	if (1.f <= SOUL_EATER_HEAL_RATIO_GAIN)
	{
		per = 1.f;
		bonus = SOUL_EATER_HEAL_RATIO_GAIN;
	}
	else
	{
		bonus = 1.f;
		per = 1.f / SOUL_EATER_HEAL_RATIO_GAIN;
	}

	float bonus2, per2;
	if (1.f <= SOUL_EATER_HEAL_RATIO_LOSE)
	{
		per2 = 1.f;
		bonus2 = SOUL_EATER_HEAL_RATIO_LOSE;
	}
	else
	{
		bonus2 = 1.f;
		per2 = 1.f / SOUL_EATER_HEAL_RATIO_LOSE;
	}

	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You heal for %.1lf Health per soul you gain, and %.1lf health per soul you lose",
		bonus,
		bonus2);
#pragma warning(suppress : 4172)
	return temp;
}

void SOUL_EATER::Initialize(void* input)
{
	// Set owner
	SOUL_EATER::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	//// Add the consume function
	//(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(SOUL_EATER::Consume);
	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SOUL_EATER::HealFromSouls);
}

void SOUL_EATER::Consume(void* input)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(SOUL_EATER::_OWNER);
	if (playerComp && !_SC_IN_SHOP)
	{
		int consume = (int)std::ceilf(playerComp->GetSouls() * _SC_FACTOR);

		playerComp->UpdateSouls(-consume);
	}
}

void SOUL_EATER::HealFromSouls(void* data)
{
	RelicInput::OnSoulUpdate* input = (RelicInput::OnSoulUpdate*)data;

	// Get stats
	StatComponent* stats = registry.GetComponent<StatComponent>(SOUL_EATER::_OWNER);

	// Heal from the damage dealt
	if (0 < input->soulDelta)
		stats->ApplyHealing(SOUL_EATER_HEAL_RATIO_GAIN * input->soulDelta);
	else
		stats->ApplyHealing(SOUL_EATER_HEAL_RATIO_LOSE * std::abs(input->soulDelta));
}
