#include "Relics\Offensive\ExplodingWeapon.h"
#include "Relics\Utility\RelicInternalHelper.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "Relics\Utility\RelicParticleHelper.h"
#include "CombatFunctions.h"
#include "Components.h"
#include "Registry.h"

#define EXPLODING_WEAPON_AOE_SIZE (10.f)
#define EXPLODING_WEAPON_SFX_DURATION (.15f)
#define EXPLODING_WEAPON_DAMAGE_FRACTION (.5f)

EntityID EXPLODING_WEAPON::_OWNER;

const char* EXPLODING_WEAPON::Description()
{
	static char temp[RELIC_DATA_DESC_SIZE];
	sprintf_s(temp, "Whenever you hit an enemy with an attack, enemies in a small area around that enemy also take %ld%% of that attack's damage as flat damage",
		PERCENT(EXPLODING_WEAPON_DAMAGE_FRACTION));
#pragma warning(suppress : 4172)
	return temp;
}

void EXPLODING_WEAPON::Initialize(void* input)
{
	// Set the relic's owner
	EXPLODING_WEAPON::_OWNER = *((EntityID*)input);

	// Make sure the relic function map exists
	_validateRelicFunctions();

	// Add the DoT to the weapon
	(*_RelicFunctions)[FUNC_ON_DAMAGE_APPLY].push_back(EXPLODING_WEAPON::Explosion);
}

void _EW_Particles_Begin(EntityID& entity, const int& index)
{
	registry.AddComponent<ParticleComponent>(entity, EXPLODING_WEAPON_SFX_DURATION, EXPLODING_WEAPON_AOE_SIZE, 2.5f, 0.f, 0.f, -10.f,
		2.0f, 0.50f, .0f, //color
		50/*256*/, CIRCLE_FIELD);
}


void EXPLODING_WEAPON::Explosion(void* data)
{
	RelicInput::OnDamageCalculation* input = (RelicInput::OnDamageCalculation*)data;

	// Check if it is the right entity that is attacking
	if (EXPLODING_WEAPON::_OWNER.index != input->attacker.index)
		return;

	ParticleAtEntityLocation(input->defender, EXPLODING_WEAPON_SFX_DURATION, _EW_Particles_Begin);

	TransformComponent* ownerTrans = registry.GetComponent<TransformComponent>(EXPLODING_WEAPON::_OWNER);
	float explosionDamage = input->CollapseNoCap() * EXPLODING_WEAPON_DAMAGE_FRACTION;

	for (auto entity : View<EnemyComponent>(registry))
	{
		// The transform of the other entity
		TransformComponent* otherTrans = registry.GetComponent<TransformComponent>(entity);

		if (entity.index != input->defender.index && DistanceBetweenTransforms(ownerTrans, otherTrans) < EXPLODING_WEAPON_AOE_SIZE)
		{
			// Flat damage
			Combat::HitFlat(entity, registry.GetComponent<StatComponent>(entity), explosionDamage);
		}
	}
}
