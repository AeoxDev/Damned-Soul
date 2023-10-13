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
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);

	if (!stat || !transform || !dac || !anim) //Invalid entity doesn't have the required components
		return;

	// Loop using DT
	anim->aAnim = ANIMATION_ATTACK;
	anim->aAnimIdx = 0;
	anim->aAnimTime += GetDeltaTime() * 5.0f;
	anim->aAnimTime -= anim->aAnimTime > 1.f ? 1.f : 0.f;

	//Move player quickly in the relevant direction
	transform->positionX += dac->x * (stat->moveSpeed * dac->dashModifier) * GetDeltaTime();
	transform->positionZ += dac->z * (stat->moveSpeed * dac->dashModifier) * GetDeltaTime();
}
