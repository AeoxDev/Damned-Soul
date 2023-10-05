#pragma once
#include "EntityFramework.h"

struct TimedEventComponent;
/// <summary>Creates a timed event on entityID that runs functions with the chosen entity in its function arguments.
/// This component will destroy itself once all give functoin have ran and end has been reached</summary>
/// <param name="entityID: ">The component will be created on this entity</param>
/// /// <param name="targetEntity: ">The entity to be used as an argument in the start function, e.g. startFunction(targetEntity)</param>
/// /// <param name="startTimer: ">How many in game seconds for the start timer to play once</param>
/// /// <param name="startFunction: ">The function to be called, use function in the EventFunctions folder for this</param>
void AddTimedEventComponent(EntityID& entityID, EntityID& startEntity, float startTime, void* startFunction);