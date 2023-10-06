#include "EventFunctions.h"

#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"

#include "CollisionFunctions.h" //AttackCollision

void PlayerLoseControl(EntityID& entity)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	registry.RemoveComponent<ControllerComponent>(entity); //Danger
	
	//Temp: Aggressive dash, make the player trigger AttackCollision until the end of the dash
	SetHitboxActive(entity, playerComp->attackHitboxID, true);
}

void PlayerRegainControl(EntityID& entity)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	registry.AddComponent<ControllerComponent>(entity); //Lord have mercy

	//Guh..?
	SetHitboxActive(entity, playerComp->attackHitboxID, false);
}

void PlayerDash(EntityID& entity)
{
	//Get access to players relevant components
	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	DashArgumentComponent* dac = registry.GetComponent<DashArgumentComponent>(entity);

	if (!stat || !transform || !dac) //Invalid entity doesn't have the required components
		return;

	//Move player quickly in the relevant direction
	transform->positionX += dac->x * (stat->moveSpeed * dac->dashModifier) * GetDeltaTime();
	transform->positionZ += dac->z * (stat->moveSpeed * dac->dashModifier) * GetDeltaTime();
}