#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include <assert.h>

void RandomPosition(EntityID& entity)
{
	TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);

	assert(tc != nullptr);
	tc->positionX = (float)(rand() % 32);
	tc->positionZ = (float)(rand() % 32);
}