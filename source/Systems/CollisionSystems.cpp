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
		for (auto entity : View<ControllerComponent, TransformComponent>(registry))
		{
			TransformComponent* p = registry.GetComponent<TransformComponent>(entity);
			ControllerComponent* pl = registry.GetComponent< ControllerComponent>(entity);
			//We have found a player component with a transform
			//Now take position and translate to pixel on texture and check if stage, if not, reset pos for now
			int r = PixelValueOnPosition(geoCo, p->positionX, p->positionZ);
			if (r == 0)
			{
				p->positionX = 0.f;
				p->positionZ = 0.f;
				pl->moveTime = 0.0f;
			}
		}
	}
	
	return true;
}