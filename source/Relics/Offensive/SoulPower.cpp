#include "Relics\Offensive\SoulPower.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"

#define SOUL_POWER_SOUL_FACTOR_PLAYER (0.01f)
#define SOUL_POWER_SOUL_FACTOR_ENEMY (.2f)

EntityID SOUL_POWER::_OWNER;

void SOUL_POWER::Initialize(void* input)
{
	// Set owner
	SOUL_POWER::_OWNER = *((EntityID*)input);

	// This is a stat altering relic, mark the entity as having modified stats
	registry.GetComponent<StatComponent>(SOUL_POWER::_OWNER)->MarkAsModified();

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add the modify health function to the stat calc functions
	(*_RelicFunctions)[FUNC_ON_STAT_CALC].push_back(SOUL_POWER::ModifyStrength);
}

void SOUL_POWER::ModifyStrength(void* data)
{
	RelicInput::OnStatCalcInput* input = (RelicInput::OnStatCalcInput*)data;

	// Check if this is the owner
	if (input->entity.index == SOUL_POWER::_OWNER.index)
	{
		// Get stats
		StatComponent* stats = (StatComponent*)input->adressOfStatComonent;
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(SOUL_POWER::_OWNER);
		if (player)
		{
			// Increase health based on souls
			stats->UpdateBonusDamage(SOUL_POWER_SOUL_FACTOR_PLAYER * player->GetSouls() * stats->GetBaseDamage());
		}
		else
		{
			EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(SOUL_POWER::_OWNER);
			stats->UpdateBonusDamage(SOUL_POWER_SOUL_FACTOR_ENEMY * enemy->soulCount * stats->GetBaseDamage());
		}
	}
}