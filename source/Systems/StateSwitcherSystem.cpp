#include "Systems\Systems.h"
#include "Levels/LevelHelper.h"
#include "Components.h"
#include "Registry.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "CollisionFunctions.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "UIComponents.h"
#include "UIRenderer.h"
#include "MemLib\ML_String.hpp"
#include "CombatFunctions.h"
#include <string>

int nrEnemies = 0;

int GetNrEnemies()
{
	return nrEnemies;
}

void StartPlayerDeath(EntityID& entity, const int& index)
{
	ResetSquashStretch(entity, index);
	AnimationComponent* animComp = registry.GetComponent<AnimationComponent>(entity);
	if (animComp != nullptr)
	{
		animComp->aAnim = ANIMATION_DEATH;
		animComp->aAnimIdx = 0;
		animComp->aAnimTime = 0.01f;
	}
	BlendAnimationComponent* blendAnimComp = registry.GetComponent<BlendAnimationComponent>(entity);
	if (blendAnimComp != nullptr)
	{
		blendAnimComp->lower.aAnim = ANIMATION_DEATH;
		blendAnimComp->lower.aAnimIdx = 0;
		blendAnimComp->lower.aAnimTime = 0.0f;

		blendAnimComp->upper.aAnim = ANIMATION_DEATH;
		blendAnimComp->upper.aAnimIdx = 0;
		blendAnimComp->upper.aAnimTime = 0.0f;
	}

	ControllerComponent* controller = registry.GetComponent<ControllerComponent>(entity);
	if (controller)
	{
		registry.RemoveComponent<ControllerComponent>(entity);
	}

	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	transform->currentSpeedX = 0.0f;
	transform->currentSpeedZ = 0.0f;

	//registry.RemoveComponent<HitboxComponent>(entity);
	RemoveHitbox(entity, 0);
	RemoveHitbox(entity, 1);
	RemoveHitbox(entity, 2);
	RemoveHitbox(entity, 3);
	RemoveHitbox(entity, 4);
	
}

void PlayPlayerDeath(EntityID& entity, const int& index)
{
	AnimationComponent* animComp = registry.GetComponent<AnimationComponent>(entity);
	if (animComp != nullptr)
	{
		animComp->aAnim = ANIMATION_DEATH;
		animComp->aAnimIdx = 0;
		float scalar = 3.0f * GetTimedEventElapsedTime(entity, index) / GetTimedEventTotalTime(entity, index);
		animComp->aAnimTime = 0.01f + scalar;
		if (scalar > 1.0f)
		{
			animComp->aAnimTime = 0.99999f;
		}
	}
	BlendAnimationComponent* blendAnimComp = registry.GetComponent<BlendAnimationComponent>(entity);
	if (blendAnimComp != nullptr)
	{
		blendAnimComp->lower.aAnim = ANIMATION_DEATH;
		blendAnimComp->upper.aAnim = ANIMATION_DEATH;

		blendAnimComp->lower.aAnimIdx = 0;
		blendAnimComp->upper.aAnimIdx = 0;

		float scalar = 3.0f * GetTimedEventElapsedTime(entity, index) / GetTimedEventTotalTime(entity, index);
		blendAnimComp->lower.aAnimTime = 0.01f + scalar;
		blendAnimComp->upper.aAnimTime = 0.01f + scalar;
		if (scalar > 1.0f)
		{
			blendAnimComp->lower.aAnimTime = 0.99999f;
			blendAnimComp->upper.aAnimTime = 0.99999f;
		}
	}
}

void EndPlayerDeath(EntityID& entity, const int& index)
{
	UpdateScoreBoardUI(false);
}

bool StateSwitcherSystem::Update()
{
	PlayerComponent* playersComp = nullptr;

	// Get player entity stat component
	if (stateManager.player.index != -1)
	{
		EntityID player = stateManager.player;
		playersComp = registry.GetComponent<PlayerComponent>(player);
		StatComponent* statComp = registry.GetComponent<StatComponent>(player);
		ControllerComponent* controller = registry.GetComponent<ControllerComponent>(player);
		if (statComp != nullptr && controller != nullptr)
		{
			if (statComp->GetHealth() <= 0 && currentStates & State::InPlay && registry.GetComponent<BlendAnimationComponent>(player)->lower.aAnim != ANIMATION_DEATH) //Added a check to see if the player death animation is already playing (Joaquin)
			{
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(player);
				if (sfx != nullptr)
				{
					sfx->Play(Player_Death, Channel_Base);
					if (stateManager.activeLevel != stateManager.finalLevel)
					{
						int soundToPlay = rand() % 3;
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
						for (auto lucifer : View<LuciferBehaviour>(registry))
						{
							SoundComponent* otherSfx = registry.GetComponent<SoundComponent>(lucifer);
							if (otherSfx)
							{
								otherSfx->Stop(Channel_Base);
								otherSfx->Play(Boss_YouveBeenJudged, Channel_Extra);
							}

							//Player death sound (Make a timed event to play after boss victory sound.)
							TimedEventIgnoreGamespeed(true);
							AddTimedEventComponentStart(stateManager.player, 4.0f, PlayBossVictoryOrDeathLine, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
						}
					}
				}
				//Timed Event for player death animation and state transitioning;
				ReleaseTimedEvents(player);
				AddTimedEventComponentStartContinuousEnd(player, 0.0f, StartPlayerDeath, PlayPlayerDeath, 3.0f, EndPlayerDeath);
			}
		}
	}
	int previousEnemies = nrEnemies;
	nrEnemies = 0;
	
	for (auto entity : View<EnemyComponent, StatComponent>(registry))
	{
		++nrEnemies;
		// Get enemy entity stat component
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
		//Get enemy sound component
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if (statComp->GetHealth() <= 0 && statComp->performingDeathAnimation == false)
		{
			bool notZac = true;
			TempBossBehaviour* tempBossComp = registry.GetComponent<TempBossBehaviour>(entity);
			if (tempBossComp == nullptr)
			{
				statComp->performingDeathAnimation = true;
				if (playersComp != nullptr)
				{
					playersComp->killingSpree += 1;
				}
				float shatterTimeFactor = 1.0f;
				if (statComp->overkill > statComp->GetMaxHealth() * 0.3f + 10.0f)
				{
					shatterTimeFactor = 0.01f;
				}
				// start timed event for MURDER
				switch (registry.GetComponent<EnemyComponent>(entity)->type)
				{
				case EnemyType::hellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					shatterTimeFactor *= 0.55f;
					break;
				case EnemyType::skeleton:
					sfx->Play(Skeleton_Death, Channel_Base);
					shatterTimeFactor *= 0.5f;
					break;
				case EnemyType::empoweredSkeleton:
					sfx->Play(Skeleton_Death, Channel_Base);
					shatterTimeFactor *= 0.5f;
					break;
				case EnemyType::empoweredImp:
					sfx->Play(Imp_Death, Channel_Base);
					shatterTimeFactor *= 1.f;
					break;
				case EnemyType::empoweredHellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					shatterTimeFactor *= 0.55f;
					break;
				case EnemyType::frozenEye:
					sfx->Play(Eye_Death, Channel_Base);
					shatterTimeFactor *= 0.f;
					break;
				case EnemyType::frozenHellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					shatterTimeFactor *= 0.f;
					break;
				case EnemyType::frozenImp:
					sfx->Play(Imp_Death, Channel_Base);
					shatterTimeFactor *= 0.f;
					break;
				case EnemyType::zac:
					sfx->Play(Miniboss_Death, Channel_Base);
					shatterTimeFactor *= 0.f;
					break;
				case EnemyType::eye:
					sfx->Play(Eye_Death, Channel_Base);
					shatterTimeFactor *= 0.9f;
					break;
				case EnemyType::imp:
					sfx->Play(Imp_Death, Channel_Base);
					shatterTimeFactor *= 1.f;
					break;
				case EnemyType::minotaur:
					sfx->Play(Minotaur_Death, Channel_Base);
					shatterTimeFactor *= 0.85f;
					break;
				case EnemyType::lucifer:
					sfx->Play(Boss_Death, Channel_Base);
					sfx->Play(Boss_MustNotDie, Channel_Extra);
					shatterTimeFactor *= 0.f;

					//Player victory sound (Make a timed event to play after boss death sound.)
					TimedEventIgnoreGamespeed(true);
					AddTimedEventComponentStart(stateManager.player, 5.5f, PlayBossVictoryOrDeathLine, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
					break;
				}

				ModelBonelessComponent* model = nullptr;
				model = registry.GetComponent<ModelBonelessComponent>(entity);
				if (model != nullptr)
				{
					model->shared.hasOutline = false;
				}

				ModelSkeletonComponent* model2 = nullptr;
				model2 = registry.GetComponent<ModelSkeletonComponent>(entity);
				if (model2 != nullptr)
				{
					model2->shared.hasOutline = false;
				}

				if (registry.GetComponent<DebuffComponent>(entity) != nullptr)
				{
					DamageNumbersDOTRemainder(entity);
					registry.RemoveComponent<DebuffComponent>(entity);
				}
				AddTimedEventComponentStartContinuousEnd(entity, 0.f, nullptr, nullptr, shatterTimeFactor, ShatterEnemy);
				AddTimedEventComponentStartContinuousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, shatterTimeFactor + 0.5f, RemoveEnemy);
				
			}
			else // boss died lmao
			{
				statComp->performingDeathAnimation = true;
				if (playersComp != nullptr)
				{
					playersComp->killingSpree += 1;
				}
				if (tempBossComp->deathCounter < 3) //spawn new mini russian doll skeleton
				{

					sfx->Play(Miniboss_Reassemble, Channel_Base);
					// start timed event for new little bossies
					AddTimedEventComponentStartContinuousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, SplitBoss);
				}
				else // le snap
				{
					// start timed event for MURDER
				
					AddTimedEventComponentStartContinuousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, RemoveEnemy);
				}
			}
		}
		
	}
	//this is test code for ending game loop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (playersComp != nullptr)
	{
		bool endGameLoop = true;
		for (auto enemyEntity : View<EnemyComponent>(registry))
		{
			endGameLoop = false;
			continue;
		}
		if ((GetGodModePortal() || endGameLoop) && !playersComp->portalCreated && !(currentStates & State::InShop) && !(currentStates & State::InMainMenu))
		{
			if (stateManager.gate.index != -1)
			{
				registry.DestroyEntity(stateManager.gate);
				stateManager.gate.index = -1;
			}
			SetGodModePortal(false);
			playersComp->portalCreated = true;
			if (stateManager.activeLevel == stateManager.finalLevel)//Final stage
			{
				OnCollisionParameters c = { 0 };
				c.entity2 = stateManager.player;
				LoadNextLevel(c);
			}
			else
			{
				EntityID portal = registry.CreateEntity();
				CreatePortal(portal, 0);//Create portal immediately instead of using a timed event.
			}

			//If it's on the split boss stage play a voice line from the player.
			if (stateManager.activeLevel == 7)
			{
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(stateManager.player);
				if (sfx != nullptr)
				{
					int soundToPlay = rand() % 3;
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
			else if (stateManager.activeLevel != stateManager.finalLevel)
			{
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(stateManager.player);
				if (sfx != nullptr)
				{
					bool isPlaying = false;
					for (auto entity : View<AudioEngineComponent>(registry))
					{
						if (registry.GetComponent<AudioEngineComponent>(entity)->channels[sfx->channelIndex[Channel_Extra]]->isPlaying(&isPlaying))
						{
							int soundToPlay = rand() % 3;
							switch (soundToPlay)
							{
							case 0:
								sfx->Play(Player_TimeToGo, Channel_Extra);
									break;
							case 1:
								sfx->Play(Player_GetGoing, Channel_Extra);
									break;
							case 2:
								sfx->Play(Player_ConvenientGates, Channel_Extra);
								break;
							}
						}
					}
				}
			}
		}
	}

	return true;
}