#pragma once

//Condition 0 = no special condition for timed event functions
#define CONDITION_DASH ((uint32_t)1)
#define CONDITION_DAMAGE ((uint32_t)2)

struct EntityID;
struct TimedEventComponent;
/// <summary>Creates a timed event on entityID that runs functions with the chosen entity in its function arguments.
/// This component will destroy itself once all give functoin have ran and end has been reached</summary>
/// <param name="entityID: ">The component will be created on this entity</param>
/// /// <param name="targetEntity: ">The entity to be used as an argument in the start function, e.g. startFunction(targetEntity)</param>
/// /// <param name="startTimer: ">How many in game seconds for the start timer to play once</param>
/// /// <param name="startFunction: ">The function to be called, use function in the EventFunctions folder for this</param>
void AddTimedEventComponentStart(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction);
//Adds a start and an end event. Use functions from the EventFunctions folder.
void AddTimedEventComponentStartEnd(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& endEntity, float endTime, void* endFunction);
void AddTimedEventComponentStartContinous(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& continousEntity, float continousTime, void* continousFunction);
void AddTimedEventComponentStartContinuousEnd(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& continousEntity, void* continousFunction,
	EntityID& endEntity, float endTime, void* endFunction, uint32_t condition = 0);
void AddTimedEventComponentStartContinuousEnd(EntityID& entityID, float startTime, void* startFunction, void* continousFunction, float endTime, void* endFunction, uint32_t condition = 0);

uint32_t GetTimedEventCondition(TimedEventComponent*& comp);

//Returns -1.0f if no component.
float GetEventTimedElapsed(EntityID& entityID);