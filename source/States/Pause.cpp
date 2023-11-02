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
#include "UIButton.h"
#include "UI/UIButtonFunctions.h"

void PauseState::Setup()
{
	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
}

void PauseState::Input()
{
	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		if (currentStates & State::InShop)
		{
			SetInPause(false);
			SetInShop(true);
			RedrawUI();
			ResetInput();

			Unload(ENT_PERSIST_PAUSE);
		}
		else
		{
			SetInPause(false);
			SetInPlay(true);
			RedrawUI();
			gameSpeed = 1.0f;
			ResetInput();

			Unload(ENT_PERSIST_PAUSE);
		}
	}
}

void PauseState::SetupButtons()
{
	//Start Button
	{
		auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", "Resume", UIFunc::PauseState_ResumeGame, { -0.81f, -0.28f }, { 0.7f, 0.6f });
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}

	//Settings Button
	{
		auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", "Settings (NYI)", UIFunc::MainMenu_Settings, { -0.81f,  -0.54f }, { 0.7f, 0.6f });
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}

	//Exit Button
	{
		auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", "Main Menu", UIFunc::PauseState_MainMenu, { -0.81f, -0.8f }, { 0.7f, 0.6f });
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}
}

void PauseState::SetupImages()
{
	// Damned Soul Main Menu Title
	auto title = registry.CreateEntity(ENT_PERSIST_PAUSE);
	auto tc = registry.AddComponent<UIImage>(title);
	tc->Setup("ExMenu/DamnedTitle3.png", { 0.0f, 0.20f }, { 1.0f, 1.0f });
}

void PauseState::SetupText()
{
	auto pause = registry.CreateEntity(ENT_PERSIST_PAUSE);
	auto pc = registry.AddComponent<UIText>(pause);
	pc->Setup("Game Paused");
}

void PauseState::Unload(int unloadPersistent)
{
	UnloadEntities((ENTITY_PERSISTENCY_TIER)unloadPersistent);
}