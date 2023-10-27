#include "States\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"
#include "Components.h"
#include "Input.h"
#include "States\StateManager.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "CollisionFunctions.h"
#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Camera.h"
#include "Light.h"
#include "UIComponents.h"

#include "MemLib\ML_String.hpp"

#include "Level.h"

// Relic Stuff
#include "RelicFunctions.h"

void GameScene::Input(bool isShop)
{
	if (isShop)
	{
		if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
		{
			SetInMainMenu(true);
			SetInPlay(false);
			SetInShop(false);
			Unload(true);
			stateManager.menu.Setup();
		}
	}
	else
	{
		if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
		{
			SetInMainMenu(true);
			SetInPlay(false);
			SetInShop(false);
			Unload(true);
			stateManager.menu.Setup();
		}

	}
}

void GameScene::Update()
{
	
}

void GameScene::ComputeShaders()
{
	/*Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();*/
}

void GameScene::Unload(bool unloadPersistent)
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	UnloadEntities((ENTITY_PERSISTENCY_TIER)unloadPersistent);
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload(true);
	//Relics::ClearRelicFunctions();
	stateManager.menu.Setup();
}
