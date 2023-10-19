#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"

struct TimedEventComponent
{
	uint32_t condition = 0; //In case we want to define some extra condition as to how functions should be called. 0 means nothing
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

void AddTimedEventComponentStartContinuousEnd(EntityID& entityID, float startTime, void* startFunction, void* continousFunction, float endTime, void* endFunction, uint32_t condition)
{
	AddTimedEventComponentStartContinuousEnd(entityID, entityID, startTime, startFunction,
		entityID, continousFunction,
		entityID, endTime, endFunction, condition);
}

uint32_t GetTimedEventCondition(TimedEventComponent*& comp)
{
	return comp->condition;
}

float GetEventTimedElapsed(EntityID& entityID)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entityID);
	if (comp)
	{
		return comp->timer;
	}
	return -1.0f;
}

void AddTimedEventComponentStartContinuousEnd(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& continousEntity, void* continousFunction,
	EntityID& endEntity, float endTime, void* endFunction, uint32_t condition)
{
	TimedEventComponent* tc = registry.AddComponent<TimedEventComponent>(entityID);
	tc->condition = condition;
	tc->startEntity = startEntity;
	tc->startTime = startTime;
	tc->startFunction = (void(*)(EntityID&))startFunction;

	tc->continousEntity = continousEntity;
	tc->continousFunction = (void(*)(EntityID&))continousFunction;

	tc->endEntity = endEntity;
	tc->endTime = endTime;
	tc->endFunction = (void(*)(EntityID&))endFunction;
}


