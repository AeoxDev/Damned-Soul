#pragma once
#include<cinttypes>
//Condition 0 = no special condition for timed event functions
#define CONDITION_DASH ((uint32_t)1)
#define CONDITION_CHARGE ((uint32_t)2)
//#define CONDITION_DAMAGE ((uint32_t)2)

struct EntityID;
struct TimedEventComponent;
/// <summary>Creates a timed event on entityID that runs functions with the chosen entity in its function arguments.
/// This component will destroy itself once all give functoin have ran and end has been reached</summary>
/// <param name="entityID: ">The component will be created on this entity</param>
/// /// <param name="targetEntity: ">The entity to be used as an argument in the start function, e.g. startFunction(targetEntity)</param>
/// /// <param name="startTimer: ">How many in game seconds for the start timer to play once</param>
/// /// <param name="startFunction: ">The function to be called, use function in the EventFunctions folder for this</param>
int AddTimedEventComponentStart(EntityID& entityID, float startTime, void* startFunction, uint32_t condition = 0, int maxStacks = 1);
//Adds a start and an end event. Use functions from the EventFunctions folder.
int AddTimedEventComponentStartEnd(EntityID& entityID, float startTime, void* startFunction,
	float endTime, void* endFunction, uint32_t condition = 0, int maxStacks = 1);
int AddTimedEventComponentStartContinous(EntityID& entityID, float startTime, void* startFunction,
	float continousTime, void* continousFunction, int maxStacks = 1);
int AddTimedEventComponentStartContinuousEnd(EntityID& entityID, float startTime, void* startFunction,
	void* continousFunction, float endTime, void* endFunction, uint32_t condition = 0, int maxStacks = 1);

uint32_t GetTimedEventCondition(EntityID& entity, const int& timedEventSlot);
uint32_t GetTimedEventCondition(TimedEventComponent*& comp, const int& timedEventSlot);

//Returns -1.0f if no component.
float GetTimedEventElapsedTime(EntityID& entityID, const int& timedEventSlot);
float GetTimedEventTotalTime(EntityID& entityID, const int& timedEventSlot);
void CancelTimedEvent(EntityID& entityID, const int& timedEventSlot);
void ReleaseTimedEvents(EntityID& entity);