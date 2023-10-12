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

	EntityID continousEntity;//endity
	void(*continousFunction)(EntityID& entity) = nullptr;//Calls function(entity)

	float endTime = 0.0f;
	EntityID endEntity;//endity
	void(*endFunction)(EntityID& entity) = nullptr;//Calls function(entity)
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
		}
		if (comp->continousFunction != nullptr && comp->startTime < comp->timer && comp->timer < comp->endTime)
		{
			comp->continousFunction(comp->continousEntity);
		}
		if (comp->endFunction != nullptr && comp->endTime < comp->timer)
		{
			comp->endFunction(comp->endEntity);
			comp->endFunction = nullptr;
		}
		if (comp->endTime < comp->timer)
		{
			registry.RemoveComponent<TimedEventComponent>(entity);
		}
	}
	return true;
}

void AddTimedEventComponentStart(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction)
{
	TimedEventComponent* tc = registry.AddComponent<TimedEventComponent>(entityID);
	tc->startEntity = startEntity;
	tc->startTime = startTime;
	tc->endTime = startTime;
	tc->startFunction = (void(*)(EntityID&))startFunction;
}
//Adds a start and an end event. Use functions from the EventFunctions folder.
void AddTimedEventComponentStartEnd(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& endEntity, float endTime, void* endFunction)
{
	TimedEventComponent* tc = registry.AddComponent<TimedEventComponent>(entityID);
	tc->startEntity = startEntity;
	tc->startTime = startTime;
	tc->startFunction = (void(*)(EntityID&))startFunction;

	tc->endEntity = endEntity;
	tc->endTime = endTime;
	tc->endFunction = (void(*)(EntityID&))endFunction;
}
void AddTimedEventComponentStartContinous(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& continousEntity, float continousTime, void* continousFunction)
{
	TimedEventComponent* tc = registry.AddComponent<TimedEventComponent>(entityID);
	tc->startEntity = startEntity;
	tc->startTime = startTime;
	tc->startFunction = (void(*)(EntityID&))startFunction;

	tc->continousEntity = continousEntity;
	tc->endTime = continousTime;
	tc->continousFunction = (void(*)(EntityID&))continousFunction;
}

void AddTimedEventComponentStartContinousEnd(EntityID& entityID, float startTime, void* startFunction, void* continousFunction, float endTime, void* endFunction)
{
	AddTimedEventComponentStartContinousEnd(entityID, entityID, startTime, startFunction,
		entityID, continousFunction,
		entityID, endTime, endFunction);
}

void AddTimedEventComponentStartContinousEnd(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& continousEntity, void* continousFunction,
	EntityID& endEntity, float endTime, void* endFunction)
{
	TimedEventComponent* tc = registry.AddComponent<TimedEventComponent>(entityID);
	tc->startEntity = startEntity;
	tc->startTime = startTime;
	tc->startFunction = (void(*)(EntityID&))startFunction;

	tc->continousEntity = continousEntity;
	tc->continousFunction = (void(*)(EntityID&))continousFunction;

	tc->endEntity = endEntity;
	tc->endTime = endTime;
	tc->endFunction = (void(*)(EntityID&))endFunction;
}


