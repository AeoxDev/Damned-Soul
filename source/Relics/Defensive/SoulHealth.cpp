#include "Relics\Defensive\SoulHealth.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\SoulRelicHelper.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_HEALTH_SOUL_FACTOR_PLAYER (3.f)
#define SOUL_HEALTH_SOUL_FACTOR_ENEMY (10.f)

EntityID SOUL_HEALTH::_OWNER;

const char* SOUL_HEALTH::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You gain %.1lf Maximum Health for every soul in your possession"/*, but you lose %ld%% of your current souls (rounded up) at the start of each level"*/,
		SOUL_HEALTH_SOUL_FACTOR_PLAYER/*,
		PERCENT(_SC_FACTOR)*/);
#pragma warning(suppress : 4172)
	return temp;
}

void SOUL_HEALTH::Initialize(void* input)
{
	// Set owner
	SOUL_HEALTH::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(SOUL_HEALTH::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	//// Add the consume function
	//(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(SOUL_HEALTH::Consume);
	// Update current health and UI
	(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SOUL_HEALTH::CompensateSoulLoss);
	// Add the modify health function to the stat calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(SOUL_HEALTH::ModifyHealth);
}

void SOUL_HEALTH::Consume(void* input)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(SOUL_HEALTH::_OWNER);
	if (playerComp && !_SC_IN_SHOP)
	{
		int consume = (int)std::ceilf(playerComp->GetSouls() * _SC_FACTOR);

		playerComp->UpdateSouls(-consume);
	}
}

void SOUL_HEALTH::CompensateSoulLoss(void* data)
{
	RelicInput::OnSoulUpdate* input = (RelicInput::OnSoulUpdate*)data;

	if (input->soulDelta < 0.f)
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(SOUL_HEALTH::_OWNER);
 		stats->StealthilyModifyHealth(-SOUL_HEALTH_SOUL_FACTOR_PLAYER * input->soulDelta);
	}
}

void SOUL_HEALTH::ModifyHealth(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == SOUL_HEALTH::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(SOUL_HEALTH::_OWNER);
		if (player)
		{
			// Increase health based on souls
			stats->UpdateBonusHealth(SOUL_HEALTH_SOUL_FACTOR_PLAYER * player->GetSouls());
		}
		else
		{
			EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(SOUL_HEALTH::_OWNER);
			stats->UpdateBonusHealth(SOUL_HEALTH_SOUL_FACTOR_ENEMY * enemy->soulCount);
		}
	}
}