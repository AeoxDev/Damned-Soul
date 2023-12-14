#include "Relics\Gadget\LightningGod.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "CombatFunctions.h"
#include "MemLib/ML_Vector.hpp"

EntityID LIGHTNING_GOD::_OWNER;
EntityID _LG_Victim;

#define LIGHTNING_GOD_COOLDOWN_SECONDS (3.0f)
#define LIGHTNING_GOD_DAMAGE_FLAT (15.f)
#define LIGHTNING_GOD_MARK_RANGE (60.f)
#define LIGHTNING_GOD_SFX_DURATION_SMALL (0.25f)
#define LIGHTNING_GOD_SFX_SIZE_SMALL (0.125f)
#define LIGHTNING_GOD_SFX_DURATION_BIG (0.6f)
#define LIGHTNING_GOD_SFX_SIZE_BIG (0.275f)

const char* LIGHTNING_GOD::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Marks a nearby enemy for %.1lf seconds every %.1lf seconds. When striking a marked enemy, your attack damage is increased by %.0lf",
		LIGHTNING_GOD_COOLDOWN_SECONDS,
		LIGHTNING_GOD_COOLDOWN_SECONDS,
		LIGHTNING_GOD_DAMAGE_FLAT);
#pragma warning(suppress : 4172)
	return temp;
}

void LIGHTNING_GOD::Initialize(void* input)
{
	LIGHTNING_GOD::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add the lightning bolt with cooldown to the Damage Calc vector
	(*_RelicFunctions)[FUNC_ON_DAMAGE_CALC].push_back(LIGHTNING_GOD::ProcMark);

	// Add the lightning bolt with cooldown to the Frame Update vector
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(LIGHTNING_GOD::OnUpdate);
}

void _LG_Particles_Begin_Big(EntityID& entity, const int& index)
{
	//Elliot: Adding a component this way is unsafe, a release is required if there already is a particleComponent
		//The solution: Find and release if it already exists
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr)
	{
		particle->Release();
	}
	registry.AddComponent<ParticleComponent>(entity, LIGHTNING_GOD_SFX_DURATION_BIG, 0.0f, LIGHTNING_GOD_SFX_SIZE_BIG, 0.0f, 0.0f, 0.0f, 256, LIGHTNING);
}

void LIGHTNING_GOD::ProcMark(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// If the defender has the mark, add flat damage and set the victim to "nothing"
	if (input->defender.index == _LG_Victim.index && input->attacker.index == LIGHTNING_GOD::_OWNER.index)
	{
		// Spawn Small Lightning
		ParticleAtEntityLocation(_LG_Victim, LIGHTNING_GOD_SFX_DURATION_BIG, _LG_Particles_Begin_Big);
		input->flatAdd += LIGHTNING_GOD_DAMAGE_FLAT;
		_LG_Victim = { -1, false };
	}
}

void _LG_Particles_Begin_Small(EntityID& entity, const int& index)
{
	//Elliot: Adding a component this way is unsafe, a release is required if there already is a particleComponent
		//The solution: Find and release if it already exists
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr)
	{
		particle->Release();
	}
	registry.AddComponent<ParticleComponent>(entity, LIGHTNING_GOD_SFX_DURATION_SMALL, 0.0f, 0.15f, 0.0f, 0.0f, 0.0f,
		0.0,0.7,3.0,//rgb
		256, LIGHTNING);
}

void _LG_Particles_Follow(EntityID& entity)
{
	//Elliot: Adding a component this way is unsafe, a release is required if there already is a particleComponent
		//The solution: Find and release if it already exists
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr)
	{
		particle->Release();
	}
		registry.AddComponent<ParticleComponent>(entity, 0.60f, 1.0f, 1.5f, 0.0f, 1.6f, 0.0f,
			0.0, 0.7, 3.0,//rgb
			64, NILL);

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

		// Get all "killable" things
		TransformComponent* ownerTrasnsform = registry.GetComponent<TransformComponent>(LIGHTNING_GOD::_OWNER);
		ML_Vector<EntityID> potentialVictims;
		for (auto entity : View<EnemyComponent>(registry))
		{
			if (DistanceBetweenTransforms(ownerTrasnsform, registry.GetComponent<TransformComponent>(entity)) < LIGHTNING_GOD_MARK_RANGE)
				potentialVictims.push_back(entity);
		}

		if (0 == potentialVictims.size())
		{
			// If no victim is chosen, just do nothing and set the victim to nothing
			cooldown = .25f;
			_LG_Victim = { -1, false };
			return;
		}

		// Randomly select one of them, srand is called when validating relics the first time
		int randomlySelected = ((size_t)rand()) % potentialVictims.size();

		_LG_Victim = potentialVictims[randomlySelected];
		
		// Spawn Small Lightning
		ParticleAtEntityLocation(potentialVictims[randomlySelected], LIGHTNING_GOD_SFX_DURATION_SMALL, _LG_Particles_Begin_Small);
		ParticleAtEntityFollow(potentialVictims[randomlySelected], LIGHTNING_GOD_COOLDOWN_SECONDS, /*nullptr*/ _LG_Particles_Follow);
	}
}