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

	if (Camera::InCutscene() == true)
	{
		TimedEventIgnoreGamespeed(false);
	}
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
			gameSpeed = 1.0f;
			if (Camera::InCutscene() == true)
			{
				TimedEventIgnoreGamespeed(true);
				gameSpeed = 0.0f;
			}
		}
		else
		{
			SetInPause(false);
			SetInPlay(true);

			SetPaused(false);
			gameSpeed = 1.0f;
			RedrawUI();
			if (Camera::InCutscene() == true)
			{
				TimedEventIgnoreGamespeed(true);
				gameSpeed = 0.0f;
			}
			ResetInput();

			Unload(ENT_PERSIST_PAUSE);
		}
	}
}

void PauseState::SetupButtons()
{

	const int pauseAmount = 4;
	const int settingsAmount = 8;


	//Pause Buttons
	{
		const char const texts[pauseAmount][32] =
		{
			"Game Paused",
			"Resume",
			"Settings",
			"Main Menu"
		};

		const DSFLOAT2 const positions[pauseAmount] =
		{
			{ 0.0f, 0.0f },
			{ 0.0f, 0.275f },
			{ 0.0f, 0.0f },
			{ 0.0f, -0.275f }
		};

		const DSFLOAT2 const scales[pauseAmount] =
		{
			{ 1.5f, 3.0f },
			{ 0.7f, 0.6f },
			{ 0.7f, 0.6f },
			{ 0.7f, 0.6f }
		};

		void(* const onClicks[pauseAmount - 1])(void*, int) =
		{
			UIFunctions::Pause::Resume,
			UIFunctions::Pause::SetSettings,
			UIFunctions::Pause::SetMainMenu
		};

		void(* const onHovers[pauseAmount - 1])(void*, int, bool) =
		{
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image
		};

		for (int i = 0; i < pauseAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UIPauseButtonComponent* pauseSettings = registry.AddComponent<UIPauseButtonComponent>(button);

			if (i == 0)
			{
				uiElement->Setup("Exmenu/ButtonBackgroundHover", texts[i], positions[i], scales[i]);
				uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.5f));
			}
			else
				uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

			if (i != 0)
			{
				onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, onClicks[i - 1]);
				onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onHovers[i - 1]);
			}

			SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
			sound->Load(MENU);
		}
	}

	//Settings Buttons
	{
		const char const filename[settingsAmount][32] =
		{
			"Exmenu/ButtonBackgroundHover",
			"Exmenu/ButtonBackground",
			"Slider1",
			"Slider1",
			"Slider1",
			"Slider1",
			"Slider1",
			"Exmenu/ButtonBackground"
		};

		const char const texts[settingsAmount][32] =
		{
			"Settings",
			"Enable Game Timer",
			"Master Volume",
			"Voice Volume",
			"SFX Volume",
			"Music Volume",
			"Ambient Volume",
			"Back"
		};

		const DSFLOAT2 const positions[settingsAmount] =
		{
			{ 0.0f, 0.0f },
			{ -0.4f, 0.4f },
			{ 0.3f, 0.3f },
			{ 0.3f, 0.2f },
			{ 0.3f, 0.1f },
			{ 0.3f, 0.0f },
			{ 0.3f, -0.1f },
			{ -0.4f, -0.4f }
		};

		const DSFLOAT2 const scales[settingsAmount] =
		{
			{ 3.0f, 3.0f },
			{ 0.4f, 0.4f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 0.6f, 0.6f }
		};

		void(* const onClicks[settingsAmount - 1])(void*, int) =
		{
			UIFunctions::Settings::SwitchTimer,
			UIFunctions::OnClick::None,
			UIFunctions::OnClick::None,
			UIFunctions::OnClick::None,
			UIFunctions::OnClick::None,
			UIFunctions::OnClick::None,
			UIFunctions::Pause::Back
		};

		void(* const onHovers[settingsAmount - 1])(void*, int, bool) =
		{
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::None,
			UIFunctions::OnHover::None,
			UIFunctions::OnHover::None,
			UIFunctions::OnHover::None,
			UIFunctions::OnHover::None,
			UIFunctions::OnHover::Image
		};

		for (int i = 0; i < settingsAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UIPauseSettingsComponent* pauseSettings = registry.AddComponent<UIPauseSettingsComponent>(button);

			uiElement->Setup(filename[i], texts[i], positions[i], scales[i]);
			if (i == 0)
				uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.5f));

			uiElement->SetAllVisability(false);

			if (i != 0)
			{
				onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, onClicks[i - 1]);
				onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onHovers[i - 1]);
			}

			SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
			sound->Load(MENU);
		}
	}
}

void PauseState::SetupImages()
{

}

void PauseState::SetupText()
{
	auto pause = registry.CreateEntity(ENT_PERSIST_PAUSE);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(pause);
	uiElement->Setup("TempShopTitle", "", { 0.0f, 0.6f });
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
	if (Camera::InCutscene() == true)
	{
		TimedEventIgnoreGamespeed(true);
		gameSpeed = 0.0f;
	}
}