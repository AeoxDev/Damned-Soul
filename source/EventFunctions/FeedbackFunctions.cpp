#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"
#include "Camera.h"

#define SHAKINESS 0.2f
#define TIME_FACTOR 256.f

#define CAMERA_SHAKE_SPEED 32.0f
#define CAMERA_SHAKE_CONSTANT 0
#define CAMERA_SHAKE_CONSTANT_STRENGTH 0.0035f

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

void ShakeCamera(EntityID& entity, const int& index)
{
	TimedEventComponent* timed = registry.GetComponent<TimedEventComponent>(entity);
	int condition = GetTimedEventCondition(timed, index);
	if (condition == CAMERA_SHAKE_CONSTANT)
	{
		Camera::SetOffset(0.0f, cosf(GetEventTimedElapsed(entity, index) * CAMERA_SHAKE_SPEED) * CAMERA_SHAKE_CONSTANT_STRENGTH, 0.0f);
	}
	
}

void ResetCameraOffset(EntityID& entity, const int& index)
{
	TimedEventComponent* timed = registry.GetComponent<TimedEventComponent>(entity);
	int condition = GetTimedEventCondition(timed, index);
	Camera::SetOffset(0.0f, 0.0f, 0.0f);
}
