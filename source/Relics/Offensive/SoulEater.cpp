#include "Relics\Offensive\SoulEater.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_EATER_HEAL_RATIO (.5f)

EntityID SOUL_EATER::_OWNER;

const char* SOUL_EATER::Description()
{
	float bonus, per;
	if (1.f <= SOUL_EATER_HEAL_RATIO)
	{
		per = 1.f;
		bonus = SOUL_EATER_HEAL_RATIO;
	}
	else
	{
		bonus = 1.f;
		per = 1.f / SOUL_EATER_HEAL_RATIO;
	}

	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You heal for %.1lf Health for every %.1lf souls you gain or lose",
		bonus,
		per);
#pragma warning(suppress : 4172)
	return temp;
}

void SOUL_EATER::Initialize(void* input)
{
	// Set owner
	SOUL_EATER::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SOUL_EATER::HealFromSouls);
}

void SOUL_EATER::HealFromSouls(void* data)
{
	RelicInput::OnSoulUpdate* input = (RelicInput::OnSoulUpdate*)data;

	// Get stats
	StatComponent* stats = registry.GetComponent<StatComponent>(SOUL_EATER::_OWNER);

	// Heal from the damage dealt
	stats->ApplyHealing(SOUL_EATER_HEAL_RATIO * std::abs(input->soulDelta));
}
