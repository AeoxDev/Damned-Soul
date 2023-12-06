#include "EventFunctions.h"

#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"

#include "CollisionFunctions.h" //AttackCollision
#include "Backend/Collision.h" //Off the deep end

#include "Relics/RelicFunctions.h"

static float godModeFactor = 1.0f;
bool godModePortal = false;
void SetGodModeFactor(float value)
{
	godModeFactor = value;
}
float GetGodModeFactor()
{
	return godModeFactor;
}
bool GetGodModePortal()
{
	return godModePortal;
}
void SetGodModePortal(bool createPortal)
{
	godModePortal = createPortal;
}
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

/*
* NOTES FROM TESTING V2 (MORE ON THE BALANCING SIDE):
* SOULS/ECONOMY:
* The economy in the game is kinda scuffed, we earn too many souls for how cheap things are in the shop. Big conversation needs to be had about the increasing and decreasing of numbers
* I personally think all numbers should be bumped up. Regular skeleton for instance giving 12 souls (5 of them on level 1 results in 60 souls once you enter the first shop)
* 
* DAMAGE:
* I think damage numbers are fairly reasonable right now, when we don't take relics into account. All relics that increase our damage need to be nerfed, since they all stack
* This stacking leads to numbers spiralling out of control *so quickly*. An alternate solution is diminishing returns but I think we've kinda voted against that in the past.
* 
* DEFENSE:
* Don't let life steal apply to reflected damage. Plain and simple. If you get reduction + reflect + lifesteal it's over. Hold down left-click and you win.
* 
* LIFESTEAL(HEALING IN GENERAL):
* Lifesteal has been a problem since day one, and imo shouldn't scale with your damage.
* Because we have so many ways to bump up our damage, it becomes exponentially better with time.
* A flat, low number will be much better and more balanced, and still useful. 
* HOWEVER, in the current state of the game, it'd make lifesteal useless compared to the regular "Heal" option in the shop.
* This heal should have its cost go up in three ways. 
*		First, the base cost should be higher, 2 souls is ridiculous. 
*		Second, the cost should go up over the course of the game
*		Third, the cost should go up depending on your maximum health. 
* Stacking health relics and buying 25% of your max hp back after every level for just 2 souls is ridiculous (or better yet, full heal for 8 souls)
* If you get Soul Health early you can just buy that and then only use souls for healing in shop for the rest of the game, easymode
* 
* SOUL-RELICS:
* Generally scuffed. If you find any of them in shop you can buy them and never buy anything else and it's gg, you're scaling until lategame
* Numbers need to be tweaked(even more if souls gained go up). Should also be capped (idk if they already are)
*/

/*
* NOTES FROM TESTING V3 (NEWER LEVELS):
* CAMERA:
* Camera should be zoomed in, as well as *work* (aiming is completely off right now. EDIT: Elliot fixed the biggest issue)
*
* SCALE:
* Depending on how much the camera is zoomed in, models should be scaled up because all that TA work for models that can't even be seen
*
* MAP:
* Loving the new maps, but they FEEL a bit too large imo. I think in tandem with zooming the camera and scaling the models up, speeds should go up as well so it doesn't take too long running around the arena
* On the topic of running around the arena, not knowing where the final few enemies are is pretty annoying so maybe some sort of indicator so we know where they're at?
* Maybe the increased model size + increased speed will fix that issue but just a thought
* Also: Big objects like pillars or huge skulls can end up hiding not only us the player, but also enemies (such as imps, shooting us from out of nowhere)
* Some sort of transparency is needed so we can still see stuff (I think the zoomed-in camera will help this feel more natural)
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
	ControllerComponent* control = registry.GetComponent<ControllerComponent>(entity);
	if (control != nullptr)
	{
		registry.RemoveComponent<ControllerComponent>(entity);
	}
	
	
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
			SetHitboxCanDealDamage(entity, playerComp->dashHitboxID, true);
		}

		AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
		anim->aAnimTimeFactor = 5.f;

		stats->hazardModifier = 0.0f;//Make the player immune to hazards during dash.

		if (playerComp->isAttacking)
		{
			//Cancel attack
			SetHitboxActive(entity, playerComp->attackHitboxID, false);
			playerComp->isAttacking = false;
		}

		if (playerComp->currentCharge > 0.0f) //Dash cancelling a charged attack
		{
			playerComp->currentCharge = 0.0f; //Reset charged attack, well spotted arian
			if(stats)
				stats->SetSpeedMult(1.0f); //Reset the speed too, phew
		}
		


		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		DashArgumentComponent* dac = registry.GetComponent<DashArgumentComponent>(entity);
		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
		playerComp->isDashing = true;
 		transform->currentSpeedX += dac->x * (stat->m_acceleration * dac->dashModifier);// * GetDeltaTime();
		transform->currentSpeedZ += dac->z * (stat->m_acceleration * dac->dashModifier);// *GetDeltaTime();
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
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	AttackArgumentComponent* aac = registry.GetComponent<AttackArgumentComponent>(entity);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);

	if (!anim || !stats || !aac || !player)
		return;

	//Animations have 1 second default duration and as such we scale the speed of the animation here so it fits the duration we pass in
	//Duration * X = 1.0f
	//1.0f / Duration = X
	float speedDiff = 1.0f / aac->duration; //stats->GetAttackSpeed() instead of 1.0f

	//speedDiff *= stats->GetAttackSpeed(); //Speed up the animation further based on attack speed
	anim->aAnimTimeFactor = speedDiff; //Cracked
	anim->aAnimTime = 0.0f; //reset animation

	stats->SetSpeedMult(0.6f); //Move slower while attacking
	player->isAttacking = true;
	ResetAttackTrackerFlags(entity);
}

void PlayBossVictoryOrDeathLine(EntityID& entity, const int& index)
{
	//Do the playing here
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
	if (sfx != nullptr)
	{
		int soundToPlay = rand() % 3;

		if (registry.GetComponent<StatComponent>(entity)->GetHealth() <= 0.0f)
		{
			switch (soundToPlay)
			{
			case 0:
				sfx->Play(Player_NotAgain, Channel_Extra);
				break;
			case 1:
				sfx->Play(Player_GetRevenge, Channel_Extra);
				break;
			case 2:
				sfx->Play(Player_HellSucks, Channel_Extra);
				break;
			}
		}
		else
		{
			switch (soundToPlay)
			{
			case 0:
				sfx->Play(Player_SuckOnThat, Channel_Extra);
				break;
			case 1:
				sfx->Play(Player_WinnerIs, Channel_Extra);
				break;
			case 2:
				sfx->Play(Player_HellYeah, Channel_Extra);
				break;
			}
		}
	}
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
		playerComp->isDashing = false;
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
	player->hasActivatedHitbox = false; //Reset

	// ## ALEX CODE ##
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr)
	{
		particle->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}
		
	// ## EO ALEX CODE ##

	if (registry.GetComponent<ChargeAttackArgumentComponent>(entity) != nullptr)
		registry.RemoveComponent<ChargeAttackArgumentComponent>(entity);

	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	stats->SetSpeedMult(1.0f); //Reset

	//Bugfix: Getting hit during the end of the attack animation cancels out the part where the attack hitbox should be inactivated
	SetPlayerAttackHitboxInactive(entity, index);
}

void PlayerAttack(EntityID& entity, const int& index)
{
	//All we do right now is perform the attack animation
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);

	if (player->isAttacking == false)
		return; //NICE TRY BUDDY

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
		
	else if (animTime >= HITBOX_START_TIME && player->hasActivatedHitbox == false) //hasActivatedHitbox ensures we only enable once, now for dash-cancel reasons
	{
		SetPlayerAttackHitboxActive(entity, index);
		player->hasActivatedHitbox = true;
		
		// ## ALEX CODE ##
		ParticleComponent* pSlashComp = registry.AddComponent<ParticleComponent>(entity, 5.0f, 50.0f, 1.0f, 0.0f + (transform->facingX * 3.0f), 5.0f, 0.0f + (transform->facingZ * 3.0f), 1, "\\SwordSlash.mdl", VFX_PATTERN::SWORD);
		// ## EO ALEX CODE ##

	}
	else if(animTime >= HITBOX_START_TIME)
	{
		float softCollisionRadius = GetHitboxRadius(entity, 1);
		float hitboxTime = (animTime - HITBOX_START_TIME) / (HITBOX_END_TIME - HITBOX_START_TIME);
		float width = (.03f + std::min(.5f, hitboxTime * 3.f)) * softCollisionRadius * HITBOX_SCALE * GetGodModeFactor(); //.2f changed to .1f
		float depth = (0.3f + std::min(1.f, hitboxTime * 2.f)) * softCollisionRadius * HITBOX_SCALE * GetGodModeFactor();
		ConvexReturnCorners corners = GetHitboxCorners(entity, player->attackHitboxID);


		// Counter clockwise
		// X
		corners.cornersX[0] = -width;
		corners.cornersX[1] = width;
		corners.cornersX[2] = 2.0f * width;
		corners.cornersX[3] = -2.0f * width;
		// Z
		corners.cornersZ[0] = -2.f * depth;
		corners.cornersZ[1] = -2.f * depth;
		corners.cornersZ[2] = -0.5f;
		corners.cornersZ[3] = -0.5f;

		SetHitboxCorners(entity, player->attackHitboxID, corners.cornerCount, corners.cornersX, corners.cornersZ);
	}
}

void PlayerDashSound(EntityID& entity, const int& index)
{
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
	sfx->Play(Player_Dash, Channel_Base);
}

void HurtSound(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	if (enemy != nullptr)
	{
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		switch (enemy->type)
		{
		case EnemyType::hellhound:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Hellhound_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::eye:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Eye_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::skeleton:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Skeleton_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::empoweredSkeleton:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Skeleton_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::empoweredImp:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Imp_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::empoweredHellhound:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Hellhound_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::frozenEye:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Eye_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::frozenHellhound:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Hellhound_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::frozenImp:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Imp_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::lucifer:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				AudioEngineComponent* audioJungle = nullptr;
				for (auto audioEngine : View<AudioEngineComponent>(registry))
				{
					audioJungle = registry.GetComponent<AudioEngineComponent>(audioEngine);
				}
				bool isPlaying = false;
				if (audioJungle)
				{
					audioJungle->channels[sfx->channelIndex[Channel_Extra]]->isPlaying(&isPlaying);
					bool hurtSoundPlaying = false;
					FMOD::Sound* test = nullptr;
					audioJungle->channels[sfx->channelIndex[Channel_Extra]]->getCurrentSound(&test);
					if (test == audioJungle->sounds[BOSS6] || test == audioJungle->sounds[BOSS7] || test == audioJungle->sounds[BOSS8] || test == audioJungle->sounds[BOSS9] || test == audioJungle->sounds[BOSS10])
					{
						hurtSoundPlaying = true;
					}
					if (!isPlaying || hurtSoundPlaying)
					{
						int soundToPlay = rand() % 5;
						switch (soundToPlay)
						{
						case 0:
							sfx->Play(Boss_Hurt1, Channel_Extra);
							break;
						case 1:
							sfx->Play(Boss_Hurt2, Channel_Extra);
							break;
						case 2:
							sfx->Play(Boss_Hurt3, Channel_Extra);
							break;
						case 3:
							sfx->Play(Boss_Hurt4, Channel_Extra);
							break;
						case 4:
							sfx->Play(Boss_Hurt5, Channel_Extra);
							break;
						}
					}
				}
			}
			break;
		case EnemyType::tempBoss:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Miniboss_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::zac:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Miniboss_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::imp:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Imp_Hurt, Channel_Extra);
			}
			break;
		case EnemyType::minotaur:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Minotaur_Hurt, Channel_Extra);
			}
			break;
		}
	}
	else
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		if (player != nullptr)
		{
			bool isPlaying = false;
			for (auto audioEngine : View<AudioEngineComponent>(registry))
			{
				//Check if sound is currently playing
				registry.GetComponent<AudioEngineComponent>(audioEngine)->channels[registry.GetComponent<SoundComponent>(entity)->channelIndex[Channel_Extra]]->isPlaying(&isPlaying);
			}
			registry.GetComponent<SoundComponent>(entity)->Play(Player_Hurt, Channel_Extra);
		}
	}
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
	anim->aAnim = ANIMATION_ATTACK;
	//anim->aAnimTime += GetDeltaTime() * anim->aAnimTimeFactor;
	anim->aAnimIdx = 1;

	//anim->aAnimTime += GetDeltaTime() * 2.0f; //Double speed animation
	//anim->aAnimTime -= anim->aAnimTime > 1.f ? 1.f : 0.f;
	
	//Move player quickly in the relevant direction
	//transform->currentSpeedX = dac->x * (stat->GetSpeed() * dac->dashModifier);// * GetDeltaTime();
	//transform->currentSpeedZ = dac->z * (stat->GetSpeed() * dac->dashModifier);// *GetDeltaTime();
	//transform->currentSpeedX
}