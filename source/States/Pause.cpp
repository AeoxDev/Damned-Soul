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

	if (Camera::InCutscene() > 0)
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

			gameSpeed = 1.0f;
			Unload(ENT_PERSIST_PAUSE);
			
		}
		else
		{
			SetInPause(false);
			SetInPlay(true);

			SetPaused(false);

			RedrawUI();
			ResetInput();

			gameSpeed = 1.0f;
			Unload(ENT_PERSIST_PAUSE);
		}
	}
}

void PauseState::SetupButtons()
{

	const int pauseAmount = 5;
	const int settingsAmount = 7;
	const int sliderAmount = 5;

	//Pause Buttons
	{
		const char texts[pauseAmount][32] =
		{
			"Game Paused",
			"Resume",
			"Settings",
			"Main Menu",
			""
		};

		const DSFLOAT2 positions[pauseAmount] =
		{
			{ 0.0f, 0.0f },
			{ -0.285f, 0.225f },
			{ -0.285f, 0.0f },
			{ -0.285f, -0.225f },
			{ 0.23f, 0.0f}

		};

		const DSFLOAT2 scales[pauseAmount] =
		{
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		void(* const onClicks[pauseAmount])(void*, int) =
		{
			UIFunctions::OnClick::None,
			UIFunctions::Pause::Resume,
			UIFunctions::Pause::SetSettings,
			UIFunctions::Pause::SetMainMenu,
			UIFunctions::OnClick::None
		};

		void(* const onHovers[pauseAmount])(void*, int, bool) =
		{
			UIFunctions::OnHover::None,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::None
		};

		for (int i = 0; i < pauseAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
			
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UIPauseButtonComponent* pauseSettings = registry.AddComponent<UIPauseButtonComponent>(button);

			if (i == 0)
			{
				if (stateManager.activeLevel > 10)
					uiElement->Setup("BookFrozen60%", texts[i], positions[i], scales[i]);
				else
					uiElement->Setup("BookNormal60%", texts[i], positions[i], scales[i]);
				uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.4f));
			}
			else if (i == 4)
			{
				uiElement->Setup("Controls", texts[i], positions[i], scales[i]);
			}
			else 
			{
				uiElement->Setup("ButtonMedium", texts[i], positions[i], scales[i]);
			}

			if (i != 0 && i != 4)
			{
				OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
				OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);

				onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onClicks[i], UIFunctions::OnClick::None);
				onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onHovers[i]);

				SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
				sound->Load(MENU);
			}
		}
	}

	//Settings Buttons
	{
		const char texts[settingsAmount][32] =
		{
			"Settings",
			"Fullscreen",
			"1920x1080",
			"1600x900",
			"1280x720",
			"Enable Game Timer",
			"Back",
		};

		const DSFLOAT2 positions[settingsAmount] =
		{
			{ 0.0f, 0.0f },
			{ -0.3f, 0.225f },
			{ -0.3f, 0.075f },
			{ -0.3f, -0.075f },
			{ -0.3f, -0.225f },
			{ 0.3f, 0.225f },
			{ 0.78f, -0.85f }
		};

		const DSFLOAT2 scales[settingsAmount] =
		{
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		const char filenames[settingsAmount][32] =
		{
			"SettingsPanel",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonMedium"
		};

		const float fontsizes[settingsAmount] =
		{
			{ 30.0f },
			{ 18.0f },
			{ 18.0f },
			{ 18.0f },
			{ 18.0f },
			{ 17.0f },
			{ 20.0f }
		};

		void(* const onClicks[settingsAmount - 1])(void*, int) =
		{
			UIFunctions::Settings::SetFullscreen,
			UIFunctions::Settings::SetHighRes,
			UIFunctions::Settings::SetMediumRes,
			UIFunctions::Settings::SetLowRes,
			UIFunctions::Settings::SwitchTimer,
			UIFunctions::Pause::Back
		};

		void(* const onHovers[settingsAmount - 1])(void*, int, bool) =
		{
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image
		};

		for (int i = 0; i < settingsAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);

			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UIPauseSettingsComponent* pauseSettings = registry.AddComponent<UIPauseSettingsComponent>(button);

			SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
			sound->Load(MENU);

			uiElement->Setup(filenames[i], texts[i], positions[i], scales[i], fontsizes[i]);

			uiElement->SetAllVisability(false);

			if (i == 0)
			{
				uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.5f));
				continue;
			}

			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);

			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onClicks[i - 1], UIFunctions::OnClick::None);
			onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onHovers[i - 1]);

		}
	}
	

	//Sliders
	{
		const char texts[sliderAmount][32] =
		{
			"Master Volume",
			"Voice Volume",
			"SFX Volume",
			"Music Volume",
			"Ambient Volume",
		};

		const DSFLOAT2 positions[sliderAmount] =
		{
			{ 0.0f, 0.225f },
			{ 0.0f, 0.075f },
			{ 0.0f, -0.075f },
			{ 0.0f, -0.225f },
			{ 0.0f, -0.375f }
		};

		AudioEngineComponent* audioComp = nullptr;

		for (auto audio : View<AudioEngineComponent>(registry))
		{
			audioComp = registry.GetComponent<AudioEngineComponent>(audio);
		}

		for (int i = 0; i < sliderAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UISettingsSliderComponent* slider = registry.AddComponent<UISettingsSliderComponent>(button);
			UIPauseSettingsComponent* pauseSettings = registry.AddComponent<UIPauseSettingsComponent>(button);

			uiElement->Setup("SliderBackground2", texts[i], positions[i]);
			uiElement->AddImage("SliderButton2", positions[i], DSFLOAT2(1.0f, 1.0f), false);
			uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(positions[i].x, positions[i].y + 0.075f));

			float maxLeftPosition = uiElement->m_BaseImage.baseUI.GetPositionBounds().left + 0.11f;
			float maxRightPosition = uiElement->m_BaseImage.baseUI.GetPositionBounds().right - 0.11f;

			uiElement->SetAllVisability(false);

			float sliderWidth = abs(maxRightPosition) + abs(maxLeftPosition);

			if (audioComp != nullptr)
			{
				float volume = 0;

				if (uiElement->m_BaseText.m_Text == "Master Volume")
				{
					audioComp->groups[MASTER_GROUP]->getVolume(&volume);
				}
				if (uiElement->m_BaseText.m_Text == "Voice Volume")
				{
					audioComp->groups[VOICE_GROUP]->getVolume(&volume);
				}
				if (uiElement->m_BaseText.m_Text == "SFX Volume")
				{
					audioComp->groups[SFX_GROUP]->getVolume(&volume);
				}
				if (uiElement->m_BaseText.m_Text == "Music Volume")
				{
					audioComp->groups[MUSIC_GROUP]->getVolume(&volume);
				}
				if (uiElement->m_BaseText.m_Text == "Ambient Volume")
				{
					audioComp->groups[AMBIENCE_GROUP]->getVolume(&volume);
				}

				if (volume < 0.5f)
				{
					uiElement->m_Images[0].baseUI.SetPosition(DSFLOAT2(uiElement->m_Images[0].baseUI.GetPosition().x - (sliderWidth * (0.5f - volume)), uiElement->m_BaseImage.baseUI.GetPosition().y));
					slider->currentPercentage = volume;
				}

				if (volume > 0.5f)
				{
					uiElement->m_Images[0].baseUI.SetPosition(DSFLOAT2(uiElement->m_Images[0].baseUI.GetPosition().x + (sliderWidth * (volume - 0.5f)), uiElement->m_BaseImage.baseUI.GetPosition().y));
					slider->currentPercentage = volume;
				}
			}


			slider->currentPosition = uiElement->m_Images[0].baseUI.GetPosition().x;
			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::Settings::Volume::Press, UIFunctions::Settings::Volume::Release);

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
	if (Camera::InCutscene() > 0)
	{
		TimedEventIgnoreGamespeed(true);
		gameSpeed = 0.0f;
	}
}