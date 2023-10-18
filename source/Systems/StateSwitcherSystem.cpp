#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"
#include "EventFunctions.h"
#include "States\StateManager.h"


bool StateSwitcherSystem::Update()
{
	PlayerComponent* playersComp = nullptr;

	// Loop through registry to find an entity that has PlayerComponent AND StatComponent
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		// Get player entity stat component
		playersComp = registry.GetComponent<PlayerComponent>(entity);
		StatComponent* statComp = registry.GetComponent<StatComponent>(entity);
		if (statComp != nullptr)
		{
			if (statComp->GetHealth() <= 0)
			{
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
				AddTimedEventComponentStartContinousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, RemoveEnemy);
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
					AddTimedEventComponentStartContinousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, SplitBoss);
				}
				else // le snap
				{
					// start timed event for MURDER
					AddTimedEventComponentStartContinousEnd(entity, 0.f, PlayDeathAnimation, PlayDeathAnimation, 2.f, RemoveEnemy);
				}
			}
		}
		
	}

	

	//this is test code for ending game loop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (playersComp != nullptr)
	{
		if (playersComp->killingSpree >= 4 && !playersComp->portalCreated && stateManager.activeLevel == 1)
		{
			playersComp->portalCreated = true;
			EntityID portal = registry.CreateEntity();
			AddTimedEventComponentStart(portal, portal, 1.0f, CreatePortal);
		}
		else if (playersComp->killingSpree >= 15 && !playersComp->portalCreated && stateManager.activeLevel == 2)
		{
			playersComp->portalCreated = true;
			EntityID portal = registry.CreateEntity();
			AddTimedEventComponentStart(portal, portal, 1.0f, CreatePortal);
		}
	}
	
	

	return true;
}