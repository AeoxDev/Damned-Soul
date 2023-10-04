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
		tc->lastPositionY = tc->positionZ;
		tc->lastPositionZ = tc->positionY;
	}
	return true;
}