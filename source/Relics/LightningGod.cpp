#include "Relics/RelicInternalHelper.h"
#include "Relics/LightningGod.h"
#include "Relics/RelicFuncInputTypes.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "MemLib/ML_Vector.hpp"
#include "EventFunctions.h"
#include <random>

#define LIGHTNING_GOD_COOLDOWN_SECONDS (3.f)
#define LIGHTNING_GOD_DAMAGE_FLAT (100.f)
#define LIGHTNING_GOD_SFX_DURATION (0.5f)

void LIGHTNING_GOD::Initialize(void* input)
{
	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add the lightning bolt with cooldown to the Frame Update vector
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(LIGHTNING_GOD::OnUpdate);
}

void _LG_Particles_Begin(EntityID& entity, const int& index)
{
	registry.AddComponent<ParticleComponent>(entity, LIGHTNING_GOD_SFX_DURATION, 0, 0.15f, 0, 0, 0, LIGHTNING);
}

void _LG_Particles_End(EntityID& entity, const int& index)
{
	//implement later, goddamn TA
	auto particles = registry.GetComponent<ParticleComponent>(entity);
	particles->Release();
	registry.DestroyEntity(entity);
}


void LIGHTNING_GOD::OnUpdate(void* data)
{
	static float cooldown = LIGHTNING_GOD_COOLDOWN_SECONDS;
	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	// Count down the cooldown
	cooldown -= input->timeDelta;
	if (cooldown < 0)
	{
		// Should always happen no matter what
		cooldown += LIGHTNING_GOD_COOLDOWN_SECONDS;

		//// Get player
		//EntityID player;
		//for (auto entity : View<PlayerComponent>(registry))
		//	player = entity;

		// Get all "killable" things
		ML_Vector<EntityID> potentialVictims;
		for (auto entity : View<EnemyComponent>(registry))
		{
			potentialVictims.push_back(entity);
		}

		if (0 == potentialVictims.size())
		{
			return;
		}

		// Randomly select one of them, srand is called when validating relics the first time
		int randomlySelected = ((size_t)rand()) % potentialVictims.size();

		// The unfortunate one about to be struck by lightning
		StatComponent* unfortunateVictim = registry.GetComponent<StatComponent>(potentialVictims[randomlySelected]);
		TransformComponent* victimTransform = registry.GetComponent<TransformComponent>(potentialVictims[randomlySelected]);

		EntityID percyJackson = registry.CreateEntity();
		TransformComponent newTrans;
		newTrans.positionX = victimTransform->positionX;
		newTrans.positionY = victimTransform->positionY;
		newTrans.positionZ = victimTransform->positionZ;
		registry.AddComponent<TransformComponent>(percyJackson, newTrans);
 		AddTimedEventComponentStartEnd(percyJackson, 0, _LG_Particles_Begin, LIGHTNING_GOD_SFX_DURATION+0.01f, _LG_Particles_End);

		unfortunateVictim->UpdateHealth(-LIGHTNING_GOD_DAMAGE_FLAT);
	}
}