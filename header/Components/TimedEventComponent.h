#pragma once

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
void AddTimedEventComponentStartContinousEnd(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction,
	EntityID& continousEntity, void* continousFunction,
	EntityID& endEntity, float endTime, void* endFunction);
void AddTimedEventComponentStartContinousEnd(EntityID& entityID, float startTime, void* startFunction, void* continousFunction, float endTime, void* endFunction);