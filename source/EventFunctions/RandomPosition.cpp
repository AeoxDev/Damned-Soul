#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include <assert.h>

void RandomPosition(EntityID& entity)
{
	srand((int)(GetDeltaTime()*2000000.0f) % 256);
	TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);

	assert(tc != nullptr);
	tc->positionX = (float)(rand() % 128) - 64.0f;
	tc->positionZ = (float)(rand() % 128) - 64.0f;
}


