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
	ML_Array<ML_String, 3> texts;
	texts[0] = "\nResume";
	texts[1] = "\nSettings";
	texts[2] = "\nMain Menu";

	ML_Array<DSFLOAT2, 3> positions;
	positions[0] = { -0.81f, -0.28f };
	positions[1] = { -0.81f,  -0.54f };
	positions[2] = { -0.81f, -0.8f };

	ML_Array<DSFLOAT2, 3> scales;
	scales[0] = { 0.7f, 0.6f };
	scales[1] = { 0.7f, 0.6f };
	scales[2] = { 0.7f, 0.6f };

	ML_Array<void(*)(void*, int), 3> functions;
	functions[0] = UIFunc::PauseState_ResumeGame;
	functions[1] = UIFunc::MainMenu_Settings;
	functions[2] = UIFunc::PauseState_MainMenu;

	for (int i = 0; i < 3; i++)
	{
		auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", texts[i].c_str(), positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
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
}

void PauseState::Unload(int unloadPersistent)
{
	UnloadEntities((ENTITY_PERSISTENCY_TIER)unloadPersistent);
}