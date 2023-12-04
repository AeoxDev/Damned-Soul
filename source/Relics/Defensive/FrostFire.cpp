#include "Relics\Defensive\FrostFire.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"
#include "KnockBackComponent.h"
#include "CombatFunctions.h"

EntityID FROST_FIRE::_OWNER;

#define FROST_FIRE_COOLDOWN (2.f)
#define FROST_FIRE_BASE_KNOCKBACK (1.25f)
#define FROST_FIRE_DAMAGE_FLAT (5.f)
#define FROST_FIRE_RANGE (10.f)
#define FROST_FIRE_SFX_DURATION (0.3f)

static float _Frost_Fire_Cooldown = FROST_FIRE_COOLDOWN;

const char* FROST_FIRE::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Releases a shockwave every %.1lf seconds that deals %.0lf damage and knocks enemies back",
		FROST_FIRE_COOLDOWN,
		FROST_FIRE_DAMAGE_FLAT);
#pragma warning(suppress : 4172)
	return temp;
}

void FROST_FIRE::Initialize(void* input)
{
	// Set owner
	FROST_FIRE::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Reset just in case
	FROST_FIRE::Reset(nullptr);

	// Add reset function to level swap list
	(*_RelicFunctions)[FUNC_ON_LEVEL_SWITCH].push_back(FROST_FIRE::Reset);

	// Add it to the list of On Obtain functions
	(*_RelicFunctions)[FUNC_ON_FRAME_UPDATE].push_back(FROST_FIRE::PushBackAndDamage);
}

void FROST_FIRE::Reset(void* data)
{
	_Frost_Fire_Cooldown = FROST_FIRE_COOLDOWN;
}

void _FF_Particles_Begin(EntityID& entity, const int& index)
{
	registry.AddComponent<ParticleComponent>(entity, FROST_FIRE_SFX_DURATION, FROST_FIRE_RANGE, 0.35f, 0.f, 0.f, 0.f, 256, true, false, CIRCLE_FIELD);
}

void FROST_FIRE::PushBackAndDamage(void* data)
{
	// Get the input
	RelicInput::OnTimeUpdate* input = (RelicInput::OnTimeUpdate*)data;

	// Reduce the cooldown
	_Frost_Fire_Cooldown -= input->timeDelta;

	// If of coodlown
	if (_Frost_Fire_Cooldown <= 0)
	{
		// Add cooldown
		_Frost_Fire_Cooldown += FROST_FIRE_COOLDOWN;

		// The transform of the owner
		TransformComponent* ownerTrans = registry.GetComponent<TransformComponent>(FROST_FIRE::_OWNER);

		// Create the beautiful particles
		ParticleAtEntityLocation(FROST_FIRE::_OWNER, FROST_FIRE_SFX_DURATION, _FF_Particles_Begin);

		for (auto entity : View<EnemyComponent>(registry))
		{
			// The transform of the other entity
			TransformComponent* otherTrans = registry.GetComponent<TransformComponent>(entity);

			if (DistanceBetweenTransforms(ownerTrans, otherTrans) < FROST_FIRE_RANGE)
			{
				float dx, dy, x, y;
				// Calculate the direction of the knockback
				CalculateKnockBackDirection(FROST_FIRE::_OWNER, entity, dx, dy);
				CalculateKnockBack(dx, dy, FROST_FIRE_BASE_KNOCKBACK, x, y);
				// Add the knockback
				AddKnockBack(entity, x, y);

				// Flat damage
				Combat::HitFlat(entity, registry.GetComponent<StatComponent>(entity), FROST_FIRE_DAMAGE_FLAT);
			}
		}
	}
}
