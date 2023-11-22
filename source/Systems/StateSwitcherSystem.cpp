#include "Systems\Systems.h"
#include "Levels/LevelHelper.h"
#include "Components.h"
#include "Registry.h"
#include "EventFunctions.h"
#include "States\StateManager.h"


bool StateSwitcherSystem::Update()
{
	PlayerComponent* playersComp = nullptr;

	// Get player entity stat component
	if (stateManager.player.index != -1)
	{
		playersComp = registry.GetComponent<PlayerComponent>(stateManager.player);
		StatComponent* statComp = registry.GetComponent<StatComponent>(stateManager.player);
		if (statComp != nullptr)
		{
			if (statComp->GetHealth() <= 0 && currentStates & State::InPlay)
			{
				registry.GetComponent<SoundComponent>(stateManager.player)->Play(Player_Death, Channel_Base);
				stateManager.GetCurrentLevel().GameOver();
			}
		}
	}
	

	for (auto entity : View<EnemyComponent, StatComponent>(registry))
	{
		
		// Get enemy entity stat component
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
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
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
				switch (registry.GetComponent<EnemyComponent>(entity)->type)
				{
				case EnemyType::hellhound:
					sfx->Play(Hellhound_Death, Channel_Base);
					break;
				case EnemyType::skeleton:
					sfx->Play(Skeleton_Death, Channel_Base);
					break;
				case EnemyType::eye:
					sfx->Play(Eye_Death, Channel_Base);
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
		if (/*playersComp->killingSpree >= playersComp->killThreshold*/ endGameLoop && !playersComp->portalCreated && !(currentStates & State::InShop))
		{
			playersComp->portalCreated = true;
			EntityID portal = registry.CreateEntity();
			AddTimedEventComponentStart(portal, 1.0f, CreatePortal);
		}
	}

	return true;
}