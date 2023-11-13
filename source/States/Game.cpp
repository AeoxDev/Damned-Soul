#include "States\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper\D3D11Helper.h"
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
#include "Relics/RelicFunctions.h"

void GameScene::Input(bool isShop)
{
	// If Shop level is loaded
	if (isShop)
	{
		if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
		{
			SetInMainMenu(true);
			SetInShop(false);
			Unload(true);

			stateManager.menu.Setup();
		}
	}
	// If Game Level is loaded
	else
	{
		if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
		{
			SetInPause(true);
			SetInPlay(false);
			
			gameSpeed = 0.0f;
			ResetInput();
			stateManager.pause.Setup();
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

void GameScene::Unload(int unloadPersistent)
{
	UnloadEntities((ENTITY_PERSISTENCY_TIER)unloadPersistent);
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload(true);
	//Relics::ResetRelics();
	stateManager.menu.Setup();
}
