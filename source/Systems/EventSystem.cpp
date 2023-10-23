#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "MemLib\ML_Vector.hpp"

struct TimedEvent
{
	unsigned long long id;
	uint32_t condition = 0; //In case we want to define some extra condition as to how functions should be called. 0 means nothing
	float timer = 0.0f;
	float startTime = 0.0f;
	EntityID eventity;
	void(*startFunction)(EntityID& entity, const int& index) = nullptr;//Calls function(entity)

	void(*continousFunction)(EntityID& entity, const int& index) = nullptr;//Calls function(entity)

	float endTime = -1.0f;
	void(*endFunction)(EntityID& entity, const int& index) = nullptr;//Calls function(entity)
};

struct TimedEventComponent
{
	ML_Vector<TimedEvent> timedEvents;
};

int CheckDuplicates(TimedEventComponent*& comp, unsigned long long id)
{
	int amount = 0;
	//Loop through and check for same function pointer
	for (unsigned i = 0; i < comp->timedEvents.size(); i++)
	{
		if (comp->timedEvents[i].id == id)
		{
			++amount;
		}
	}
	return amount;
}

bool EventSystem::Update()
{
	for (auto entity : View<TimedEventComponent>(registry))
	{
		
		auto comp = registry.GetComponent<TimedEventComponent>(entity);
		for (unsigned i = 0; i < comp->timedEvents.size(); i++)
		{
			comp->timedEvents[i].timer += GetDeltaTime();
			if (comp->timedEvents[i].startFunction != nullptr && comp->timedEvents[i].startTime < comp->timedEvents[i].timer)
			{
				comp->timedEvents[i].startFunction(comp->timedEvents[i].eventity, i);
				comp->timedEvents[i].startFunction = nullptr;
			}
			if (comp->timedEvents[i].continousFunction != nullptr && comp->timedEvents[i].startTime < comp->timedEvents[i].timer && comp->timedEvents[i].timer < comp->timedEvents[i].endTime)
			{
				comp->timedEvents[i].continousFunction(comp->timedEvents[i].eventity, i);
			}
			if (comp->timedEvents[i].endFunction != nullptr && comp->timedEvents[i].endTime < comp->timedEvents[i].timer)
			{
				comp->timedEvents[i].endFunction(comp->timedEvents[i].eventity, i);
				comp->timedEvents[i].endFunction = nullptr;
			}
			if (comp->timedEvents[i].endTime < comp->timedEvents[i].timer)
			{
				comp->timedEvents.erase(i);
			}
		}
		
	}
	return true;
}
//Check stacks for events here
int AddTimedEventComponentStart(EntityID& entityID, float startTime, void* startFunction, int maxStacks)
{
	TimedEventComponent* tc = registry.GetComponent<TimedEventComponent>(entityID);
	if (!tc)
	{
		tc = registry.AddComponent<TimedEventComponent>(entityID);
		tc->timedEvents.Initialize();
	}
	TimedEvent timedEvent;
	timedEvent.id = (unsigned long long)startFunction;
	if (CheckDuplicates(tc, (unsigned long long)startFunction) > maxStacks)
	{
		return -1;
	}
	timedEvent.eventity = entityID;
	timedEvent.startTime = startTime;
	timedEvent.endTime = startTime;
	timedEvent.startFunction = (void(*)(EntityID&, const int&))startFunction;
	tc->timedEvents.push_back(timedEvent);
	return tc->timedEvents.size() - 1;
}
//Adds a start and an end event. Use functions from the EventFunctions folder.
int AddTimedEventComponentStartEnd(EntityID& eventity, float startTime, void* startFunction, float endTime, void* endFunction, int maxStacks)
{
	TimedEventComponent* tc = registry.GetComponent<TimedEventComponent>(eventity);
	if (!tc)
	{
		tc = registry.AddComponent<TimedEventComponent>(eventity);
		tc->timedEvents.Initialize();
	}
	TimedEvent timedEvent;
	timedEvent.id = (unsigned long long)startFunction + (unsigned long long)endFunction;
	if (CheckDuplicates(tc, (unsigned long long)startFunction + (unsigned long long)endFunction) >= maxStacks)
	{
		return -1;
	}
	timedEvent.eventity = eventity;
	timedEvent.startTime = startTime;
	timedEvent.startFunction = (void(*)(EntityID&, const int&))startFunction;
	timedEvent.endTime = endTime;
	timedEvent.endFunction = (void(*)(EntityID&, const int&))endFunction;
	tc->timedEvents.push_back(timedEvent);
	return tc->timedEvents.size() - 1;
}
int AddTimedEventComponentStartContinous(EntityID& eventity, float startTime, void* startFunction,
	float continousTime, void* continousFunction, int maxStacks)
{
	TimedEventComponent* tc = registry.GetComponent<TimedEventComponent>(eventity);
	if (!tc)
	{
		tc = registry.AddComponent<TimedEventComponent>(eventity);
		tc->timedEvents.Initialize();
	}
	TimedEvent timedEvent;
	timedEvent.id = (unsigned long long)startFunction + (unsigned long long)continousFunction;
	if (CheckDuplicates(tc, (unsigned long long)startFunction + (unsigned long long)continousFunction) >= maxStacks)
	{
		return -1;
	}
	timedEvent.eventity = eventity;
	timedEvent.startTime = startTime;
	timedEvent.startFunction = (void(*)(EntityID&, const int&))startFunction;

	timedEvent.endTime = continousTime;
	timedEvent.continousFunction = (void(*)(EntityID&, const int&))continousFunction;
	tc->timedEvents.push_back(timedEvent);
	return tc->timedEvents.size() - 1;
}

int AddTimedEventComponentStartContinuousEnd(EntityID& eventity, float startTime, void* startFunction, 
	void* continousFunction, float endTime, void* endFunction, uint32_t condition, int maxStacks)
{
	TimedEventComponent* tc = registry.GetComponent<TimedEventComponent>(eventity);
	if (!tc)
	{
		tc = registry.AddComponent<TimedEventComponent>(eventity);
		tc->timedEvents.Initialize();
	}
	
	TimedEvent timedEvent;
	timedEvent.id = (unsigned long long)startFunction + (unsigned long long)continousFunction + (unsigned long long)endFunction;
	if (CheckDuplicates(tc, timedEvent.id)  >= maxStacks)
	{
		return -1;
	}
	timedEvent.eventity = eventity;
	timedEvent.startTime = startTime;
	timedEvent.startFunction = (void(*)(EntityID&, const int&))startFunction;

	timedEvent.condition = condition;

	timedEvent.continousFunction = (void(*)(EntityID&, const int&))continousFunction;

	timedEvent.endTime = endTime;
	timedEvent.endFunction = (void(*)(EntityID&, const int&))endFunction;
	tc->timedEvents.push_back(timedEvent);
	return tc->timedEvents.size() - 1;
}

uint32_t GetTimedEventCondition(TimedEventComponent*& comp, const int& timedEventSlot)
{
	return comp->timedEvents[timedEventSlot].condition;//Needs to be fixed
}

float GetEventTimedElapsed(EntityID& entityID, const int& timedEventSlot)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entityID);
	if (comp)
	{
		return comp->timedEvents[timedEventSlot].timer;
	}
	return -1.0f;
}

void CancelTimedEvent(EntityID& entity, const int& timedEventSlot)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entity);
	if (comp)
	{
		//registry.RemoveComponent<TimedEventComponent>(entity);
		comp->timedEvents.erase(timedEventSlot);
	}
}
