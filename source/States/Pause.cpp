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
	SetupUI();

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

void PauseState::SetupUI()
{
	const int pauseAmount = 3;
	const int settingsAmount = 6;
	const int sliderAmount = 5;

	//Pause Buttons
	{
		auto book = registry.CreateEntity(ENT_PERSIST_PAUSE);

		UIComponent* uiBook = registry.AddComponent<UIComponent>(book);
		UIPauseButtonComponent* pause1 = registry.AddComponent<UIPauseButtonComponent>(book);

		if (stateManager.activeLevel > 10)
			uiBook->Setup("BookFrozen60%", "", "Game Paused", { 0.0f, 0.0f });
		else
			uiBook->Setup("BookNormal60%", "", "Game Paused", { 0.0f, 0.0f });

		uiBook->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.4f));

		auto controls = registry.CreateEntity(ENT_PERSIST_PAUSE);

		UIComponent* uiControls = registry.AddComponent<UIComponent>(controls);
		UIPauseButtonComponent* pause02 = registry.AddComponent<UIPauseButtonComponent>(controls);

		uiControls->Setup("Controls80%", "", "", { 0.225f, 0.0f });

		const char texts[pauseAmount][32] =
		{
			"Resume",
			"Settings",
			"Main Menu"
		};

		const DSFLOAT2 positions[pauseAmount] =
		{
			{ -0.235f, 0.18f },
			{ -0.235f, 0.f },
			{ -0.235f, -0.18f }

		};

		const DSFLOAT2 scales[pauseAmount] =
		{
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		void(* const onClicks[pauseAmount])(void*, int) =
		{
			UIFunctions::Pause::Resume,
			UIFunctions::Pause::SetSettings,
			UIFunctions::Pause::SetMainMenu
		};

		void(* const onHovers[pauseAmount])(void*, int, bool) =
		{
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image,
			UIFunctions::OnHover::Image
		};

		for (int i = 0; i < pauseAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);
			
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UIPauseButtonComponent* pauseSettings = registry.AddComponent<UIPauseButtonComponent>(button);
			uiElement->Setup("ButtonMedium", "ButtonMediumHover", texts[i], positions[i], scales[i]);
			
			
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
			
			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onClicks[i], UIFunctions::OnClick::None);
			onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), onHovers[i]);
			
			SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
			sound->Load(MENU);
		}


	}

	//Settings Buttons
	{
		auto panel = registry.CreateEntity(ENT_PERSIST_PAUSE);

		UIComponent* uiElement = registry.AddComponent<UIComponent>(panel);
		UIPauseSettingsComponent* pauseSettings = registry.AddComponent<UIPauseSettingsComponent>(panel);
		uiElement->Setup("SettingsPanel", "", "Settings", { 0.0f, 0.0f }, { 1.0f, 1.0f }, 30.0f);
		uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.5f));

		uiElement->SetAllVisability(false);

		const char texts[settingsAmount][32] =
		{
			"Fullscreen",
			"1920x1080",
			"1600x900",
			"1280x720",
			"Enable Game Timer",
			"Back",
		};

		const DSFLOAT2 positions[settingsAmount] =
		{
			{ -0.25f, 0.225f },
			{ -0.25f, 0.075f },
			{ -0.25f, -0.075f },
			{ -0.25f, -0.225f },
			{ 0.25f, 0.225f },
			{ 0.78f, -0.85f }
		};

		const DSFLOAT2 scales[settingsAmount] =
		{
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		void(* const functions[settingsAmount])(void*, int) =
		{
			UIFunctions::Settings::SetFullscreen,
			UIFunctions::Settings::SetHighRes,
			UIFunctions::Settings::SetMediumRes,
			UIFunctions::Settings::SetLowRes,
			UIFunctions::Settings::SwitchTimer,
			UIFunctions::Pause::Back,
		};

		const char filenames[settingsAmount][32] =
		{
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonMedium"
		};

		const char filenamesHover[settingsAmount][32] =
		{
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonMediumHover"
		};

		const float fontsizes[settingsAmount] =
		{
			{ 17.0f },
			{ 17.0f },
			{ 17.0f },
			{ 17.0f },
			{ 16.0f },
			{ 20.0f }
		};

		for (int i = 0; i < settingsAmount; i++)
		{
			auto button = registry.CreateEntity(ENT_PERSIST_PAUSE);

			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UIPauseSettingsComponent* pauseSettings = registry.AddComponent<UIPauseSettingsComponent>(button);
			
			if (i == 4 && GetVisualTimer())
			{
				uiElement->Setup(filenamesHover[i], filenames[i], texts[i], positions[i], scales[i], fontsizes[i]);
				uiElement->SetAllVisability(false);
			}
			else
			{
				uiElement->Setup(filenames[i], filenamesHover[i], texts[i], positions[i], scales[i], fontsizes[i]);
				uiElement->SetAllVisability(false);
			}
		
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);

			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
			onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::Image);

			SoundComponent* sound = registry.AddComponent<SoundComponent>(button);
			sound->Load(MENU);

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

			uiElement->Setup("SliderBackground", "", texts[i], positions[i]);
			uiElement->AddImage("SliderButton", positions[i], DSFLOAT2(1.0f, 1.0f), false);
			uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(positions[i].x, positions[i].y + 0.075f));

			uiElement->SetAllVisability(false);

			float sliderWidth = abs(uiElement->m_BaseImage.baseUI.GetPositionBounds().right - 0.11f) + abs(uiElement->m_BaseImage.baseUI.GetPositionBounds().left + 0.11f);

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

	auto pause = registry.CreateEntity(ENT_PERSIST_PAUSE);
	UIComponent* uiElementP = registry.AddComponent<UIComponent>(pause);
	uiElementP->Setup("PanelSmall", "", " ", { 0.0f, -0.75f }, { 1.5f, 1.5f });
	uiElementP->m_BaseText.baseUI.SetScale({ 1.0f, 1.0f });
	uiElementP->SetAllVisability(false);

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