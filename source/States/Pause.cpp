#include "States/Pause.h"
#include "Input.h"
#include "States\StateManager.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "SDLHandler.h"
#include "Registry.h"
#include "Components.h"
#include "Model.h"

void PauseState::Setup()
{
	RedrawUI();
}

void PauseState::Input()
{
	if (keyState[SDL_SCANCODE_Q] == pressed)
	{
		SetInMainMenu(true);
		SetInPause(false);
		Unload(true);

		gameSpeed = 1.0f;
		ResetInput();

		stateManager.menu.Setup();
	}

	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		if (currentStates & State::InShop)
		{
			SetInPause(false);
			SetInShop(true);
			RedrawUI();
			gameSpeed = 1.0f;
			ResetInput();
		}
		else
		{
			SetInPause(false);
			SetInPlay(true);
			RedrawUI();
			gameSpeed = 1.0f;
			ResetInput();

		}
	}
}

void PauseState::Unload(bool unloadPersistent)
{
	UnloadEntities((ENTITY_PERSISTENCY_TIER)unloadPersistent);
}