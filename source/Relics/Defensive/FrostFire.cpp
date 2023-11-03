#include "Relics\Defensive\FrostFire.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"
#include "EventFunctions.h"
#include "KnockBackComponent.h"

#define FROST_FIRE_BASE_KNOCKBACK (1.f)

#define FROST_FIRE_SPEED_MULTIPLIER (.8f)

static bool FrostFireAvailable = true;

void FROST_FIRE::Initialize(void* input)
{
	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Mark as available immediately
	FROST_FIRE::SetAvailable(nullptr);

	// Add reset function to level swap list
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(FROST_FIRE::SetAvailable);

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_HEALTH_MODIFIED].push_back(FROST_FIRE::PushBackAndFreeze);
}

void FROST_FIRE::SetAvailable(void* data)
{
	FrostFireAvailable = true;
}

void FROST_FIRE::PushBackAndFreeze(void* data)
{
	// Get the input
	RelicInput::OnHealthUpdate* input = (RelicInput::OnHealthUpdate*)data;

	// If the health modification is not negative, Frost Fire should not trigger
	if (0 < input->hpDelta)
		return;

	EntityID player = { -1, false };
	// Get the player entity
	for (auto entity : View<PlayerComponent>(registry))
		player = entity;

	// Get player stat component and do nothing if ...
	StatComponent* playerStat = registry.GetComponent<StatComponent>(player);
	// ... the hurt component isn't the player's component
	// ... the player's remaining health is greater than 50%
	// ... Frost Fire has already activated this level
	if (playerStat != input->adressOfStatComponent || .5f < playerStat->GetHealthFraction() || false == FrostFireAvailable)
		return;

	// Set frost fire to used
	FrostFireAvailable = false;

	for (auto entity : View<EnemyComponent>(registry))
	{
		float dx, dy, v, x, y;

		CalculateKnockBackDirection(player, entity, dx, dy);
		// Flat knock back for now
		v = FROST_FIRE_BASE_KNOCKBACK;
		CalculateKnockBack(dx, dy, v, x, y);

		AddKnockBack(entity, x, y);

		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
		//stat->moveSpeed *= FROST_FIRE_SPEED_MULTIPLIER;
	}
}
