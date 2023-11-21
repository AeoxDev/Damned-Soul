#include "EventFunctions.h"

#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"

#include "CollisionFunctions.h" //AttackCollision
#include "Backend/Collision.h" //Off the deep end

#include "Relics/RelicFunctions.h"

/*
* NOTES FROM TESTING
* 
* GRAPHICS:
* Probably will become irrelevant when we have the player in here, but models are way too small right now
* Could also probably use an outline on the character models to really set them apart from the background
* Speaking of background, yep
* 
* GENERAL GAMEPLAY:
* I think a major balancing issue is how we're really underpowered at the start, then balanced (depending on relic rng) at the next level, then overpowered after that
* The best workaround I can think of is scaling enemies up with levels. Make them weaker at the start, and stronger over time. Breaking out *the graph* if anyone complains
* So if we're going to scale enemies up, relics need to be better balanced so you don't get wrecked simply because you didn't get a good relic early
* 
* UI:
* For the love of god, please give us damage numbers and enemy health bars so we can actually check the numbers and see what's stronger without just guessing
* Also let me see what relics I own by either holding down tab or entering the pause menu thanks
* And while we're on that topic, let's maybe take away the fact that the ESC-key ends the run if we use it in shop, while it pauses in-game xd
* 
* RELICS:
* I can tell lifesteal has been nerfed (or more like our base damage has been) but it still reigns supreme
* A huge issue is simply because we have a lot of relics that make us stronger as we go lower health, or we get hit, etc
* Thus, we thrive off of rushing in blindly, taking hits, losing health, and getting really high numbers, which in turn leads to more life steal
* Attack speed gained when getting hit is a bit ridiculous, want to tweak it to 1.5x instead of 2x, more precise numbers in the combat-section
* 
* The best workaround I can think of is scaling enemies up with levels. Make them weaker at the start, and stronger over time. Breaking out *the graph* if anyone complains
* So if we're going to scale enemies up, relics need to be better balanced so you don't get wrecked simply because you didn't get a good relic early
* 
* AI:
* Dogs need to stop chasing infinitely, unless we have some good way of actually negating the incoming damage (block, parry, big attack hitboxes that cancel their charge etc.)
* Alternatively, if the new dog attack animation actually STOPS THEM briefly when they reach you, then perform the attack, we can actually time a dodge to get out of harm's way
* Active hit frames of enemies (namely skeletons) after their attack animation finishes
* 
* FEEDBACK:
* Make enemies light up before they attack, further clarifying that you're in danger
* Add a hit-sound when we hit enemies separate from the sound they make when they take damage
* 
* COMBAT:
* Attack timings need to be tweaked more, currently a 3-hit combo takes 1.8 seconds (0.6+0.4+0.8), which feels reasonable enough, but a little slow in my opinion
* I'm thinking I tweak it to be more like 1.6 seconds (0.5+0.4+0.7 maybe?), and in turn making the relic-buffed version be 1.2 seconds (0.375+0.3+0.525)
* Make dash have a cooldown, obviously. It's a quick and easy fix but I'm waiting until the dogs have a solid counter
*/

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
			SetHitboxActive(entity, playerComp->dashHitboxID);
			SetHitboxCanDealDamage(entity, playerComp->dashHitboxID, true); //Dash hitbox
		}

		AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
		anim->aAnimTimeFactor = 5.f;

		stats->hazardModifier = 0.0f;//Make the player immune to hazards during dash.

		//SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, false);//Set attack hitbox to false
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		DashArgumentComponent* dac = registry.GetComponent<DashArgumentComponent>(entity);
		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
 		transform->currentSpeedX = dac->x * (stat->m_acceleration * dac->dashModifier);// * GetDeltaTime();
		transform->currentSpeedZ = dac->z * (stat->m_acceleration * dac->dashModifier);// *GetDeltaTime();
	}
}

void SetPlayerAttackHitboxActive(EntityID& entity, const int& index)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, true);
	SetHitboxActive(entity, playerComp->attackHitboxID, true);
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
	anim->aAnimTime = 0.0f; //reset animation

	stats->SetSpeedMult(0.6f); //Move slower while attacking
	player->isAttacking = true;
	ResetAttackTrackerFlags(entity);
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
		auto funcs = Relics::GetFunctionsOfType(Relics::FUNC_ON_DASH);
		for (auto& func : funcs)
		{
			SetHitboxActive(entity, playerComp->dashHitboxID, false);
			SetHitboxCanDealDamage(entity, playerComp->dashHitboxID, false); //Dash hitbox
		}
		stats->hazardModifier = stats->baseHazardModifier;
	}

	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	anim->aAnimTimeFactor = 1.f;
}

void SetPlayerAttackHitboxInactive(EntityID& entity, const int& index)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);
	SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, false);
	SetHitboxActive(entity, playerComp->attackHitboxID, false);
}

void PlayerEndAttack(EntityID& entity, const int& index)
{
	//Smile
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	anim->aAnimTimeFactor = 1.f;
	anim->aAnimTimePower = 1.f;

	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
	player->timeSinceLastAttack = 0.0f;
	player->isAttacking = false;
	player->currentCharge = 0.0f;

	if (registry.GetComponent<ChargeAttackArgumentComponent>(entity) != nullptr)
		registry.RemoveComponent<ChargeAttackArgumentComponent>(entity);

	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	stats->SetSpeedMult(1.0f); //Reset

	//Bugfix: Getting hit during the end of the attack animation cancels out the part where the attack hitbox should be inactivated
	SetPlayerAttackHitboxInactive(entity, index);
}

void PlayerAttack(EntityID& entity, const int& index)
{;
	//All we do right now is perform the attack animation
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);

	//Can't perform animation without the AnimationComponent, durr
	if (!anim)
		return;

	//Perform attack animation, woo, loop using DT
	anim->aAnim = ANIMATION_ATTACK;
	anim->aAnimIdx = 0;

#define HITBOX_START_TIME (0.45f)
#define HITBOX_END_TIME (0.8f)
#define HITBOX_SCALE (2.f)

	anim->aAnimTimePower = .5f;
	float animTime = anim->GetTimeValue();

	//Make the players' attack hitbox active during the second half of the attack animation
	if (animTime >= HITBOX_END_TIME)
	{
		SetPlayerAttackHitboxInactive(entity, index);
	}
		
	else if (animTime >= HITBOX_START_TIME) //&& false == player->hasActivatedHitbox)
	{
		SetPlayerAttackHitboxActive(entity, index);
		player->hasActivatedHitbox = true;
	}
	else
	{
		float softCollisionRadius = GetHitboxRadius(entity, 1);
		float hitboxTime = (animTime - HITBOX_START_TIME) / (HITBOX_END_TIME - HITBOX_START_TIME);
		float width = (.4f + std::min(.5f, hitboxTime * 3.f)) * softCollisionRadius * HITBOX_SCALE; //.2f changed to .4f
		float depth = (1.2f + std::min(1.f, hitboxTime * 2.f)) * softCollisionRadius * HITBOX_SCALE;
		ConvexReturnCorners corners = GetHitboxCorners(entity, player->attackHitboxID);


		// Counter clockwise
		// X
		corners.cornersX[0] = -width;
		corners.cornersX[1] = width;
		corners.cornersX[2] = width;
		corners.cornersX[3] = -width;
		// Z
		corners.cornersZ[0] = -2.f * depth;
		corners.cornersZ[1] = -2.f * depth;
		corners.cornersZ[2] = 0.5f;
		corners.cornersZ[3] = 0.5f;

		SetHitboxCorners(entity, player->attackHitboxID, corners.cornerCount, corners.cornersX, corners.cornersZ);
		//SetHitboxRadius(entity, player->attackHitboxID, (anim->aAnimTime - 0.5f) * 5);
	}
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
	//transform->currentSpeedX = dac->x * (stat->GetSpeed() * dac->dashModifier);// * GetDeltaTime();
	//transform->currentSpeedZ = dac->z * (stat->GetSpeed() * dac->dashModifier);// *GetDeltaTime();
	//transform->currentSpeedX
}