#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"
#include "CombatFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "States\StateManager.h"


void HazardDamageHelper(EntityID& victim, const float DPS)
{

	auto defenderStats = registry.GetComponent<StatComponent>(victim);

	if (0.001f < defenderStats->hazardModifier)
	{
		DamageOverTime dotComp(DPS * defenderStats->hazardModifier, 1.f);

		float finalDamage = Combat::CalculateDamage(dotComp, victim, RelicInput::DMG::DOT_HAZARD) * GetDeltaTime();

		if (0.0001f < finalDamage)
		{
			Combat::HitFlat(victim, defenderStats, finalDamage);
		}
	}
}

bool GeometryIndependentSystem::Update()
{
	//First find the GI component
	GeometryIndependentComponent* geoCo = nullptr;
	for (auto entity : View<GeometryIndependentComponent>(registry))
	{
		geoCo = registry.GetComponent<GeometryIndependentComponent>(entity);
	}
	//Then check the position of all players and enemies:
	if (geoCo != nullptr)
	{
		for (auto entity : View<TransformComponent, HitboxComponent, StatComponent>(registry))
		{
			TransformComponent* p = registry.GetComponent<TransformComponent>(entity);
			HitboxComponent* h = registry.GetComponent<HitboxComponent>(entity);
			StatComponent* stat = registry.GetComponent<StatComponent>(entity);
			AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
			//We have found a player component with a transform
			//Now take position and translate to pixel on texture and check if stage, if not, reset pos for now
			if (HitboxCanHitGI(entity))
			{
				int r = PixelValueOnPosition(geoCo,p);
				int takeDamage = 0;
				ProjectileComponent* proj = nullptr;

				switch (r)
				{
				case 0:
					p->positionX = 0.f;
					p->positionZ = 0.f;
					proj = registry.GetComponent<ProjectileComponent>(entity);
					if (proj != nullptr)
					{
						registry.DestroyEntity(entity);
					}
					break;
				case 1:
					//Footstep sound here?
					stat->m_acceleration = stat->m_baseAcceleration;
					break;
				case HAZARD_LAVA:
					if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
					{
						anim->aAnimTimeFactor = stat->lavaAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					stat->m_acceleration = stat->m_baseAcceleration * stat->lavaAccelFactor;

					HazardDamageHelper(entity, 25.f);
					//takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
					break;
				case HAZARD_CRACK:
					if (!stat->canWalkOnCrack)
					{
						//Detect edge
						//Edge direction
						p->positionX -= p->facingX * GetDeltaTime() * stat->GetSpeed();
						p->positionZ -= p->facingZ * GetDeltaTime() * stat->GetSpeed();
					}
					break;
				case HAZARD_ACID://Lava but more damage
					if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
					{
						anim->aAnimTimeFactor = stat->acidAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					stat->m_acceleration = stat->m_baseAcceleration * stat->acidAccelFactor;

					HazardDamageHelper(entity, 50.f);
					break;
				case HAZARD_ICE:
					//ICE:
					if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
					{
						anim->aAnimTimeFactor = stat->iceAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					stat->m_acceleration = stat->m_baseAcceleration * stat->iceAccelFactor;

					//HazardDamageHelper(entity, 25.f);
					//takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
					break;
				default:
					break;
				}
			}
			
		}
	}
	
	return true;
}