#include "Relics/FrostFire.h"
#include "Relics\RelicFuncInputTypes.h"
#include "Registry.h"
#include "Components.h"

// Used for cooldowns
#include <chrono>

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

	TransformComponent* playerTrans = registry.GetComponent<TransformComponent>(player);

	for (auto entity : View<TransformComponent, StatComponent>(registry))
	{
		if (entity.index != player.index)
		{
			// Knockback

			// Short Time Freeze
		}
	}

}
