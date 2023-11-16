#include "Relics\Gadget\SoulSpeed.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_SPEED_SOUL_FACTOR_PLAYER (.6f)
#define SOUL_SPEED_SOUL_FACTOR_ENEMY (6.f)

EntityID SOUL_SPEED::_OWNER;

const char* SOUL_SPEED::Description()
{
	char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "You gain %.1lf speed for every soul in your possession", SOUL_SPEED_SOUL_FACTOR_PLAYER);
	return temp;
}

void SOUL_SPEED::Initialize(void* input)
{
	// Set owner
	SOUL_SPEED::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(SOUL_SPEED::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add the modify speed function to the stat calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(SOUL_SPEED::ModifySpeed);
}

void SOUL_SPEED::ModifySpeed(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == SOUL_SPEED::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(SOUL_SPEED::_OWNER);
		if (player)
		{
			// Increase speed based on souls
			stats->UpdateBonusSpeed(SOUL_SPEED_SOUL_FACTOR_PLAYER * player->GetSouls());
		}
		else
		{
			EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(SOUL_SPEED::_OWNER);
			stats->UpdateBonusSpeed(SOUL_SPEED_SOUL_FACTOR_ENEMY * enemy->soulCount);
		}
	}
}