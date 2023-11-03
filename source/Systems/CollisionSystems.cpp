#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"


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
			//We have found a player component with a transform
			//Now take position and translate to pixel on texture and check if stage, if not, reset pos for now
			if (HitboxCanHitGI(entity))
			{
				int r = PixelValueOnPosition(geoCo,p);
				int takeDamage = 0;
				switch (r)
				{
				case 0:
					p->positionX = 0.f;
					p->positionZ = 0.f;
					break;
				case 1:
					//Footstep sound here?
					break;
				case HAZARD_LAVA:
					takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
					break;
				case HAZARD_CRACK:
					if (!stat->canWalkOnCrack)
					{
						//Detect edge
						//Edge direction

						p->positionX -= p->facingX * GetDeltaTime() * stat->moveSpeed;
						p->positionZ -= p->facingZ * GetDeltaTime() * stat->moveSpeed;
					}
					
				default:
					break;
				}
			}
			
		}
	}
	
	return true;
}