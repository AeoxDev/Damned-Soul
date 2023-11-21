#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "MemLib\ML_Vector.hpp"
#include <assert.h>

bool ignoreGameSpeed = false;

struct TimedEvent
{
	bool isActive = true;

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

	TimedEventComponent()
	{
		timedEvents.Initialize();
	}
};

int CheckDuplicates(TimedEventComponent*& comp, unsigned long long id)
{
	int amount = 0;
	//Loop through and check for same function pointer
	for (unsigned i = 0; i < comp->timedEvents.size(); i++)
	{
		if (comp->timedEvents[i].isActive && comp->timedEvents[i].id == id)
		{
			++amount;
		}
	}
	return amount;
}

bool EventSystem::Update()
{
	//Make sure continuous events aren't updating while the game is paused
	if (gameSpeed == 0.0f && ignoreGameSpeed == false)
		return true;

	for (auto entity : View<TimedEventComponent>(registry))
	{
		auto comp = registry.GetComponent<TimedEventComponent>(entity);
		for (unsigned i = 0; i < comp->timedEvents.size(); i++)
		{
			if (!comp->timedEvents[i].isActive)
			{
				continue;
			}
			if (comp->timedEvents[i].condition == CONDITION_IGNORE_GAMESPEED_SLOWDOWN && gameSpeed < 1.0f)
			{
				comp->timedEvents[i].timer += GetFrameTime();
			}
			else
			{
				comp->timedEvents[i].timer += GetDeltaTime();
			}

			if (comp->timedEvents[i].startFunction != nullptr && comp->timedEvents[i].startTime < comp->timedEvents[i].timer)
			{
				comp->timedEvents[i].startFunction(comp->timedEvents[i].eventity, i);
				if (i < comp->timedEvents.size())
				{
					comp->timedEvents[i].startFunction = nullptr;
				}
				else
				{
					continue;
				}
			}
			if (comp->timedEvents[i].continousFunction != nullptr && comp->timedEvents[i].startTime < comp->timedEvents[i].timer && comp->timedEvents[i].timer < comp->timedEvents[i].endTime)
			{
				comp->timedEvents[i].continousFunction(comp->timedEvents[i].eventity, i);
			}
			if (comp->timedEvents[i].endFunction != nullptr && comp->timedEvents[i].endTime < comp->timedEvents[i].timer)
			{
				comp->timedEvents[i].isActive = false;
				comp->timedEvents[i].endFunction(comp->timedEvents[i].eventity, i);
			}
			else if (comp->timedEvents[i].endTime < comp->timedEvents[i].timer)
			{
				comp->timedEvents[i].isActive = false;
			}
		}
		
	}
	return true;
}

//Find available slot, if no available. Create a slot
int FindAndInit(EntityID& entityID)
{
	TimedEventComponent* tc = registry.GetComponent<TimedEventComponent>(entityID);
	if (!tc)
	{
		return -1;
	}
	int slot = -1;
	for (unsigned i = 0; i < tc->timedEvents.size(); i++)
	{
		if (tc->timedEvents[i].isActive == false)
		{
			slot = i;
		}
	}
	if (slot == -1)
	{
		TimedEvent event;
		slot = (int)tc->timedEvents.push_back(event);
	}
	return slot;
}

//Check stacks for events here
int AddTimedEventComponentStart(EntityID& entityID, float startTime, void* startFunction, uint32_t condition,int maxStacks)
{
	TimedEventComponent* tc = registry.GetComponent<TimedEventComponent>(entityID);
	if (!tc)
	{
		tc = registry.AddComponent<TimedEventComponent>(entityID);
		tc->timedEvents.Initialize();
	}
	TimedEvent timedEvent;
	timedEvent.condition = condition;
	timedEvent.id = (unsigned long long)startFunction;
	if (CheckDuplicates(tc, (unsigned long long)startFunction) > maxStacks)
	{
		return -1;
	}
	timedEvent.eventity = entityID;
	timedEvent.startTime = startTime;
	timedEvent.endTime = startTime;
	timedEvent.startFunction = (void(*)(EntityID&, const int&))startFunction;
	int slot = FindAndInit(entityID);
	tc->timedEvents[slot] = timedEvent;
	return slot;
}
//Adds a start and an end event. Use functions from the EventFunctions folder.
int AddTimedEventComponentStartEnd(EntityID& eventity, float startTime, void* startFunction, float endTime, void* endFunction, uint32_t condition,int maxStacks)
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
	timedEvent.condition = condition;
	int slot = FindAndInit(eventity);
	tc->timedEvents[slot] = timedEvent;
	return slot;
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
	int slot = FindAndInit(eventity);
	tc->timedEvents[slot] = timedEvent;
	return slot;
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
	int slot = FindAndInit(eventity);
	tc->timedEvents[slot] = timedEvent;
	return tc->timedEvents.size() - 1;
}

uint32_t GetTimedEventCondition(EntityID& entity, const int& timedEventSlot)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entity);
	assert(comp != nullptr);
	return comp->timedEvents[timedEventSlot].condition;//Needs to be fixed
}

uint32_t GetTimedEventCondition(TimedEventComponent*& comp, const int& timedEventSlot)
{
	return comp->timedEvents[timedEventSlot].condition;//Needs to be fixed
}

float GetTimedEventElapsedTime(EntityID& entityID, const int& timedEventSlot)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entityID);
	if (comp)
	{
		return comp->timedEvents[timedEventSlot].timer - comp->timedEvents[timedEventSlot].startTime;
	}
	return -1.0f;
}

float GetTimedEventTotalTime(EntityID& entityID, const int& timedEventSlot)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entityID);
	if (comp)
	{
		return comp->timedEvents[timedEventSlot].endTime - comp->timedEvents[timedEventSlot].startTime;
	}
	return -1.0f;
}


void CancelTimedEvent(EntityID& entity, const int& timedEventSlot)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entity);
	if (comp)
	{
		//registry.RemoveComponent<TimedEventComponent>(entity);
		comp->timedEvents[timedEventSlot].isActive = false;
	}
}

void ReleaseTimedEvents(EntityID& entity)
{
	TimedEventComponent* comp = registry.GetComponent<TimedEventComponent>(entity);
	if (comp)
	{
		//registry.RemoveComponent<TimedEventComponent>(entity);
		comp->timedEvents.~ML_Vector();
	}
}

void HardResetTimedEvents()
{
}

void TimedEventIgnoreGamespeed(bool ignore)
{
	ignoreGameSpeed = ignore;
}
