#include "Relics\Defensive\FrostFire.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"
#include "KnockBackComponent.h"
#include "CombatFunctions.h"

EntityID FROST_FIRE::_OWNER;

#define FROST_FIRE_COOLDOWN (3.f)
#define FROST_FIRE_BASE_KNOCKBACK (2.25f)
#define FROST_FIRE_FREEZE_CHANCE (0.2f)
#define FROST_FIRE_FREEZE_DURATION (1.f)
//#define FROST_FIRE_DAMAGE_FLAT (5.f)
#define FROST_FIRE_RANGE (12.5f)
#define FROST_FIRE_SFX_DURATION (0.3f)

static float _Frost_Fire_Cooldown = FROST_FIRE_COOLDOWN;

const char* FROST_FIRE::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	//sprintf_s(temp, "Releases a shockwave every %.1lf seconds that deals %.0lf damage and knocks enemies back",
	//	FROST_FIRE_COOLDOWN,
	//	FROST_FIRE_DAMAGE_FLAT);
	sprintf_s(temp, "Releases a shockwave every %.1lf seconds that knocks enemies back and has a %ld%% chance to freeze them for %.1lf seconds",
			FROST_FIRE_COOLDOWN,
			PERCENT(FROST_FIRE_FREEZE_CHANCE),
			FROST_FIRE_FREEZE_DURATION);
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
	registry.AddComponent<ParticleComponent>(entity, FROST_FIRE_SFX_DURATION, FROST_FIRE_RANGE, 1.5f, 0.f, 0.f, -10.f,
		0.f, 1.10f, 2.0f,//rgb 
		200/*256*/, CIRCLE_FIELD);
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
				TempBossBehaviour* tempBoss = registry.GetComponent<TempBossBehaviour>(entity);
				LuciferBehaviour* lucyBoss = registry.GetComponent<LuciferBehaviour>(entity);
				ZacBehaviour* zac = registry.GetComponent<ZacBehaviour>(entity);
				if (tempBoss || lucyBoss || zac)
					continue;

				float dx, dy, x, y;
				// Calculate the direction of the knockback
				CalculateKnockBackDirection(FROST_FIRE::_OWNER, entity, dx, dy);
				CalculateKnockBack(dx, dy, FROST_FIRE_BASE_KNOCKBACK, x, y);
				// Add the knockback
				AddKnockBack(entity, x, y);

				//// Flat damage
				//Combat::HitFlat(entity, registry.GetComponent<StatComponent>(entity), FROST_FIRE_DAMAGE_FLAT);

					// Check if this is the owner and RNGesus allows the freeze
				if ((rand() % 100) < PERCENT(FROST_FIRE_FREEZE_CHANCE))
				{
					// Freeze them!
					DebuffComponent* debuff = registry.GetComponent<DebuffComponent>(entity);
					if (debuff && debuff->m_frozen.LessThan(FROST_FIRE_FREEZE_DURATION))
					{
						debuff->m_frozen = Frozen(FROST_FIRE_FREEZE_DURATION);
					}
					else if (nullptr == debuff)
					{
						registry.AddComponent<DebuffComponent>(entity, Frozen(FROST_FIRE_FREEZE_DURATION));
					}
				}
			}
		}
	}
}
