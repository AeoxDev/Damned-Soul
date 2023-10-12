#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"


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
		for (auto entity : View<TransformComponent, HitboxComponent>(registry))
		{
			TransformComponent* p = registry.GetComponent<TransformComponent>(entity);
			HitboxComponent* h = registry.GetComponent<HitboxComponent>(entity);
			//We have found a player component with a transform
			//Now take position and translate to pixel on texture and check if stage, if not, reset pos for now
			if (HitboxCanHitGI(entity))
			{
				int r = PixelValueOnPosition(geoCo, p->positionX, p->positionZ);
				if (r == 0)
				{
					int rr = PixelValueOnPosition(geoCo, p->positionX, p->positionZ);
					p->positionX = 0.f;
					p->positionZ = 0.f;
				}
			}
			
		}
	}
	
	return true;
}