#include "Relics/LightningGod.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "MemLib/ML_Vector.hpp"
#include <random>

#define LIGHTNING_GOD_COOLDOWN_SECONDS (3.f)
#define LIGHTNING_GOD_DAMAGE_FLAT (100.f)

void LIGHTNING_GOD::OnUpdate(void* data)
{
	static float cooldown = LIGHTNING_GOD_COOLDOWN_SECONDS;
	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	// Count down the cooldown
	cooldown -= input->timeDelta;
	if (cooldown < 0)
	{
		// Get player
		EntityID player;
		for (auto entity : View<PlayerComponent>(registry))
			player = entity;

		// Get all "killable" things
		ML_Vector<EntityID> potentialVictims;
		for (auto entity : View<StatComponent>(registry))
		{
			if (entity.index != player.index)
				potentialVictims.push_back(entity);
		}

		if (0 == potentialVictims.size())
		{
			cooldown += LIGHTNING_GOD_COOLDOWN_SECONDS;
			return;
		}

		// seed via delta time
		srand((unsigned)(1.0f / (input->timeDelta + 0.001f)));

		// Randomly select one of them
		int randomlySelected = ((size_t)rand()) % potentialVictims.size();

		// The unfortunate one about to be struck by lightning
		StatComponent* unfortunateVictim = registry.GetComponent<StatComponent>(potentialVictims[randomlySelected]);
		unfortunateVictim->UpdateHealth(-LIGHTNING_GOD_DAMAGE_FLAT);

		// Add the cooldown onto the remaining cooldown
		cooldown += LIGHTNING_GOD_COOLDOWN_SECONDS;
	}
}