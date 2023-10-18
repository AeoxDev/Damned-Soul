#include "EventFunctions.h"

#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"

#include "CollisionFunctions.h" //AttackCollision
#include "Backend/Collision.h" //Off the deep end

void PlayerLoseControl(EntityID& entity)
{	
	//Start by removing the players' ControllerComponent
	registry.RemoveComponent<ControllerComponent>(entity);
	
	//Get relevant components
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	TimedEventComponent* teComp = registry.GetComponent<TimedEventComponent>(entity);

	//Store any specific condition in the timed event
	uint32_t condition = GetTimedEventCondition(teComp);

	//If we're dashing, we make player invincible
	if (condition == CONDITION_DASH)
	{
		SetHitboxIsPlayer(entity, playerComp->hardHitboxID, false);
	}
}

void SetPlayerAttackHitboxActive(EntityID& entity)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	SetHitboxActive(entity, playerComp->attackHitboxID, true);
}

void PlayerRegainControl(EntityID& entity)
{
	//Start by giving back the players' ControllerComponent
	registry.AddComponent<ControllerComponent>(entity);

	//Get relevant components
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	TimedEventComponent* teComp = registry.GetComponent<TimedEventComponent>(entity);

	//Store any specific condition in the timed event
	uint32_t condition = GetTimedEventCondition(teComp);
	
	//If we've just dashed, we make player capable of taking damage again
	if (condition == CONDITION_DASH)
	{
		SetHitboxIsPlayer(entity, playerComp->hardHitboxID, true);
	}
}

void SetPlayerAttackHitboxInactive(EntityID& entity)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	SetHitboxActive(entity, playerComp->attackHitboxID, false);
}

void PlayerAttack(EntityID& entity)
{
	//All we do right now is perform the attack animation
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);

	//Can't perform animation without the AnimationComponent, durr
	if (!anim)
		return;

	//Perform attack animation, woo, loop using DT
	anim->aAnim = ANIMATION_ATTACK;
	anim->aAnimIdx = 0;
	anim->aAnimTime += GetDeltaTime();
	//anim->aAnimTime += GetDeltaTime() * 2.0f; //Double speed animation
	anim->aAnimTime -= anim->aAnimTime > 1.f ? 1.f : 0.f;
}

void PlayerDash(EntityID& entity)
{
	//Get access to players relevant components
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
	DashArgumentComponent* dac = registry.GetComponent<DashArgumentComponent>(entity);

	//Invalid entity doesn't have the required components
	if (!transform || !stat || !dac)
		return;

	//Move player quickly in the relevant direction
	transform->positionX += dac->x * (stat->moveSpeed * dac->dashModifier) * GetDeltaTime();
	transform->positionZ += dac->z * (stat->moveSpeed * dac->dashModifier) * GetDeltaTime();
}