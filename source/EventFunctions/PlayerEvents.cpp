#include "EventFunctions.h"

#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"

#include "CollisionFunctions.h" //AttackCollision
#include "Backend/Collision.h" //Off the deep end

#include "Relics/RelicFunctions.h"

void PlayerLoseControl(EntityID& entity, const int& index)
{	
	//Get relevant components
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	//If this is not the player, no need to remove controller
	if (playerComp == nullptr)
	{
		return;
	}
	//Start by removing the players' ControllerComponent
	registry.RemoveComponent<ControllerComponent>(entity);
	
	TimedEventComponent* teComp = registry.GetComponent<TimedEventComponent>(entity);

	//Store any specific condition in the timed event
	uint32_t condition = GetTimedEventCondition(teComp, index);

	//If we're dashing, we make player invincible
	if (condition == CONDITION_DASH)
	{
		//Set player soft hitbox to no longer take damage, but it'll deal damage if you have the Dash Attack relic
		SetHitboxCanTakeDamage(entity, playerComp->softHitboxID, false);

		auto funcs = Relics::GetFunctionsOfType(Relics::FUNC_ON_DASH);
		for (auto& func : funcs)
		{
			SetHitboxCanDealDamage(entity, 3, true); //Dash hitbox
		}

		AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
		anim->aAnimTimeFactor = 5.f;

		stats->hazardModifier = 0.0f;//Make the player immune to hazards during dash.
		//SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, false);//Set attack hitbox to false
	}
}

void SetPlayerAttackHitboxActive(EntityID& entity, const int& index)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, true);
}

void PlayerRegainControl(EntityID& entity, const int& index)
{
	//Hitstop fixer
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	transform->offsetX = 0.0f;
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	//Get relevant components
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	//If this is not the player, return. No controlling enemies >:(
	if (playerComp == nullptr)
	{
		return;
	}
	//Give back the players' ControllerComponent
	registry.AddComponent<ControllerComponent>(entity);
	TimedEventComponent* teComp = registry.GetComponent<TimedEventComponent>(entity);

	//Store any specific condition in the timed event
	uint32_t condition = GetTimedEventCondition(teComp, index);
	
	//If we've just dashed, we make player capable of taking damage again
	if (condition == CONDITION_DASH)
	{
		SetHitboxCanTakeDamage(entity, playerComp->softHitboxID, true);
		SetHitboxCanDealDamage(entity, 3, false); //Dash hitbox
		stats->hazardModifier = stats->baseHazardModifier;
	}

	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	anim->aAnimTimeFactor = 1.f;
}

void SetPlayerAttackHitboxInactive(EntityID& entity, const int& index)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, false);
}

void PlayerEndAttack(EntityID& entity, const int& index)
{
	//Smile
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	anim->aAnimTimeFactor = 1.f;

	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
	player->timeSinceLastAttack = 0.0f;
	player->isAttacking = false;
}

void PlayerBeginAttack(EntityID& entity, const int& index)
{
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	AttackArgumentComponent* aac = registry.GetComponent<AttackArgumentComponent>(entity);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);

	if (!sfx || !anim || !stats || !aac || !player)
		return;

	sfx->Play(Player_Attack, Channel_Base);

	//Animations have 1 second default duration and as such we scale the speed of the animation here so it fits the duration we pass in
	//Duration * X = 1.0f
	//1.0f / Duration = X
	float speedDiff = stats->GetAttackSpeed() / aac->duration;

	//speedDiff *= stats->GetAttackSpeed(); //Speed up the animation further based on attack speed
	anim->aAnimTimeFactor = speedDiff; //Cracked
	anim->aAnimTime = 0.0f;

	player->isAttacking = true;
}

void PlayerAttack(EntityID& entity, const int& index)
{;
	//All we do right now is perform the attack animation
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);

	//Can't perform animation without the AnimationComponent, durr
	if (!anim)
		return;

	//Perform attack animation, woo, loop using DT
	anim->aAnim = ANIMATION_ATTACK;
	anim->aAnimIdx = 0;
	//float adjustedTime = powf(anim->aAnimTime, .5f);

	//Make the players' attack hitbox active during the second half of the attack (between half up until eight tenths)
	AttackArgumentComponent* aac = registry.GetComponent<AttackArgumentComponent>(entity);
	if (anim->aAnimTime >= aac->duration * 0.9f)
	{
		SetPlayerAttackHitboxInactive(entity, index);
	}
	else if (anim->aAnimTime >= aac->duration * 0.5f)
	{
		SetPlayerAttackHitboxActive(entity, index);
	}
	//if (/*GetTimedEventElapsedTime(entity, index)*/adjustedTime >= 0.8f)
	//{
	//	SetPlayerAttackHitboxInactive(entity, index);
	//}
	//	
	//else if (/*GetTimedEventElapsedTime(entity, index)*/adjustedTime >= 0.5f)
	//{
	//	SetPlayerAttackHitboxActive(entity, index);
	//}
}

void PlayerDashSound(EntityID& entity, const int& index)
{
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
	sfx->Play(Player_Dash, Channel_Base);
}

void PlayerDash(EntityID& entity, const int& index)
{
	//Get access to players relevant components
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
	DashArgumentComponent* dac = registry.GetComponent<DashArgumentComponent>(entity);
	// Get animation
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);

	//Invalid entity doesn't have the required components
	if (!transform || !stat || !dac || !anim)
		return;

	//Perform attack animation, woo, loop using DT
	anim->aAnim = ANIMATION_WALK;
	//anim->aAnimTime += GetDeltaTime() * anim->aAnimTimeFactor;
	anim->aAnimIdx = 1;

	//anim->aAnimTime += GetDeltaTime() * 2.0f; //Double speed animation
	//anim->aAnimTime -= anim->aAnimTime > 1.f ? 1.f : 0.f;
	
	//Move player quickly in the relevant direction
	transform->positionX += dac->x * (stat->GetSpeed() * dac->dashModifier) * GetDeltaTime();
	transform->positionZ += dac->z * (stat->GetSpeed() * dac->dashModifier) * GetDeltaTime();
}