#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UIRenderer.h"
#include "CollisionFunctions.h" //AttackCollision


void LoopHitbox(EntityID& entity, const int& index)
{
	SetHitboxActive(entity, 0);
	AddTimedEventComponentStart(entity, 0.2f, LoopHitbox, 2);
}