#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"

bool TransformSystem::Update()
{
	
	for (auto entity : View<TransformComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		tc->lastPositionX = tc->positionX;
		tc->lastPositionY = tc->positionY;
		tc->lastPositionZ = tc->positionZ;
	}
	for (auto entity : View<TransformComponent, LightComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		//LightComponent* light = registry.GetComponent<LightComponent>(entity);
		//Use the offset from light.
		OffsetPosition(entity, transform->positionX, transform->positionY, transform->positionZ);
		OffsetFacing(entity, transform->facingX, transform->facingY, transform->facingZ);
	}
	return true;
}