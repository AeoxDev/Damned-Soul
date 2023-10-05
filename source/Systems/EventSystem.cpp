#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"

struct TimedEventComponent
{
	float timer = 0.0f;
	float startTime = 0.0f;
	EntityID startEntity;
	void(* startFunction)(EntityID& entity) = nullptr;//Calls function(entity)
};

bool EventSystem::Update()
{
	for (auto entity : View<TimedEventComponent>(registry))
	{
		auto comp = registry.GetComponent<TimedEventComponent>(entity);
		comp->timer += GetDeltaTime();
		if (comp->startFunction != nullptr && comp->startTime < comp->timer)
		{
			comp->startFunction(comp->startEntity);
			comp->startFunction = nullptr;
			//Move to end instead
			registry.RemoveComponent<TimedEventComponent>(entity);
		}
	}
	return true;
}

void AddTimedEventComponent(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction)
{
	TimedEventComponent* tc = registry.AddComponent<TimedEventComponent>(entityID);
	tc->startEntity = startEntity;
	tc->startTime = startTime;
	tc->startFunction = (void(*)(EntityID&))startFunction;
}
