#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"

#define SHAKINESS 0.2f
#define TIME_FACTOR 256.f

void HitStop(EntityID& entity, const int& index)
{
	//Shake character based on time

	float time = GetEventTimedElapsed(entity, index);
	float offsetX = SHAKINESS * sinf(time * TIME_FACTOR);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	if (transform != nullptr)
	{
		transform->offsetX = offsetX;
	}
	
}
