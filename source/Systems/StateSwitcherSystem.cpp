#include "Systems\Systems.h"
#include "Levels/LevelHelper.h"
#include "Components.h"
#include "Registry.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "CollisionFunctions.h"
#include "DeltaTime.h"
#include "Camera.h"

void StartPlayerDeath(EntityID& entity, const int& index)
{
	AnimationComponent* animComp = registry.GetComponent<AnimationComponent>(entity);
	if (animComp != nullptr)
	{
		animComp->aAnim = ANIMATION_DEATH;
		animComp->aAnimIdx = 0;
		animComp->aAnimTime = 0.0f;
	}

	registry.RemoveComponent<ControllerComponent>(entity);
}

void PlayPlayerDeath(EntityID& entity, const int& index)
{
	AnimationComponent* animComp = registry.GetComponent<AnimationComponent>(entity);
	if (animComp != nullptr)
	{
		animComp->aAnim = ANIMATION_DEATH;
		animComp->aAnimIdx = 0;
	}
}

void EndPlayerDeath(EntityID& entity, const int& index)
{
	stateManager.GetCurrentLevel().GameOver();
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
		if (statComp != nullptr)
		{
			if (statComp->GetHealth() <= 0 && currentStates & State::InPlay)
			{
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(player);
				if (sfx != nullptr)
				{
					sfx->Play(Player_Death, Channel_Base);
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
				//Timed Event for player death animation and state transitioning;
				AddTimedEventComponentStartContinuousEnd(player, 0.0f, StartPlayerDeath, PlayPlayerDeath, 1.0f, EndPlayerDeath);
			}
		}
	}
	

	for (auto entity : View<EnemyComponent, StatComponent>(registry))
	{
		
		// Get enemy entity stat component
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
		//Get enemy sound component
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if (statComp->GetHealth() <= 0 && statComp->performingDeathAnimation == false)
		{
			TempBossBehaviour* tempBossComp = registry.GetComponent<TempBossBehaviour>(entity);
			if (tempBossComp == nullptr)
			{
				statComp->performingDeathAnimation = true;
				if (playersComp != nullptr)
				{
					playersComp->killingSpree += 1;
				}
				// start timed event for MURDER
				switch (registry.GetComponent<EnemyComponent>(entity)->type)
				{
				case EnemyType::hellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					break;
				case EnemyType::skeleton:
					sfx->Play(Skeleton_Death, Channel_Base);
					break;
				case EnemyType::empoweredSkeleton:
					sfx->Play(Skeleton_Death, Channel_Base);
					break;
				case EnemyType::empoweredImp:
					sfx->Play(Imp_Death, Channel_Base);
					break;
				case EnemyType::empoweredHellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					break;
				case EnemyType::frozenEye:
					sfx->Play(Eye_Death, Channel_Base);
					break;
				case EnemyType::frozenHellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					break;
				case EnemyType::frozenImp:
					sfx->Play(Imp_Death, Channel_Base);
					break;
				case EnemyType::zac:
					sfx->Play(Miniboss_Death, Channel_Base);
					break;
				case EnemyType::eye:
					sfx->Play(Eye_Death, Channel_Base);
					break;
				case EnemyType::imp:
					sfx->Play(Imp_Death, Channel_Base);
					break;
				case EnemyType::minotaur:
					sfx->Play(Minotaur_Death, Channel_Base);
					break;
				case EnemyType::lucifer:
					//Play sound when we kill the final boss
					SoundComponent* otherSfx = registry.GetComponent<SoundComponent>(stateManager.player);
					if (otherSfx != nullptr)
					{
						int soundToPlay = rand() % 3;
						switch (soundToPlay)
						{
						case 0:
							otherSfx->Play(Player_SuckOnThat, Channel_Extra);
							break;
						case 1:
							otherSfx->Play(Player_WinnerIs, Channel_Extra);
							break;
						case 2:
							otherSfx->Play(Player_HellYeah, Channel_Extra);
							break;
						}	
					}
					break;
				}
				AddTimedEventComponentStartContinuousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, RemoveEnemy);
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
		if ((GetGodModePortal() || endGameLoop) && !playersComp->portalCreated && !(currentStates & State::InShop))
		{
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
				AddTimedEventComponentStart(portal, 1.0f, CreatePortal);

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
			else
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