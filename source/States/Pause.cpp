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
#include "UI/UIButtonFunctions.h"
#include "UIComponents.h"

#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

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

			SetPaused(false);

			RedrawUI();
			ResetInput();

			Unload(ENT_PERSIST_PAUSE);
		}
		else
		{
			SetInPause(false);
			SetInPlay(true);

			SetPaused(false);

			RedrawUI();
			gameSpeed = 1.0f;
			ResetInput();

			Unload(ENT_PERSIST_PAUSE);
		}
	}
}

void PauseState::SetupButtons()
{
	const char const texts[3][32] =
	{
		"\nResume",
		"\nSettings (WIP)",
		"\nMain Menu"
	};

	const DSFLOAT2 const positions[3] =
	{
		{ -0.81f, -0.28f },
		{ -0.81f,  -0.54f },
		{ -0.81f, -0.8f }
	};

	const DSFLOAT2 const scales[3] =
	{
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f }
	};

	void(* const functions[3])(void*, int) =
	{
		UIFunc::PauseState_ResumeGame,
		UIFunc::MainMenu_Settings,
		UIFunc::PauseState_MainMenu
	};

	for (int i = 0; i < 3; i++)
	{
		auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

		SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
		sound->Load(MENU);
	}

}

void PauseState::SetupImages()
{
	// Damned Soul Main Menu Title
	auto title = registry.CreateEntity(ENT_PERSIST_PAUSE);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(title);
	uiElement->Setup("ExMenu/DamnedTitle3", "", { 0.0f, 0.20f });
}

void PauseState::SetupText()
{
	auto pause = registry.CreateEntity(ENT_PERSIST_PAUSE);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(pause);
	uiElement->Setup("TempShopTitle", "Game Paused", { 0.0f, 0.43f });
	uiElement->m_BaseImage.baseUI.SetVisibility(false);

	uiElement->AddImage("TempRelicHolder", { 1.5f, 1.5f }, { 1.5f, 1.5f }, false);
	uiElement->m_Images[0].baseUI.SetVisibility(false);

	uiElement->AddText(" ", uiElement->m_Images[0].baseUI.GetBounds(), { 1.5f, 1.5f });
	uiElement->m_Texts[0].baseUI.SetVisibility(false);

	UIPauseRelicTextComponent* relicText = registry.AddComponent<UIPauseRelicTextComponent>(pause);

}

void PauseState::Unload(int unloadPersistent)
{
	UnloadEntities((ENTITY_PERSISTENCY_TIER)unloadPersistent);
}