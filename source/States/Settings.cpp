#include "States\Settings.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "UI/UIButtonFunctions.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "UIComponents.h"
#include "Model.h"
#include "Levels\LevelHelper.h"


void SettingsState::Setup()
{

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();

	Camera::ResetCamera();
	// Stage Model
	StageSetupVariables stageVars;
	
	stageVars.stageNr = rand()% 10;
	EntityID stage = SetUpStage(stageVars);
	// Stage Transform
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	// Stage POI
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);
	stageP->mode = POI_FORCE;
	stageP->height = CAMERA_OFFSET_Y * -0.85f;
	stageP->rotationY = 0.0f;
	stageP->rotationRadius = -0.7f * CAMERA_OFFSET_Z;
	stageP->rotationAccel = 0.12f;
}

void SettingsState::Input()
{

}

void SettingsState::SetupButtons()
{
	const int amount = 7;
	const int sliderAmount = 5;

	//Buttons
	{
		const char texts[amount][32] =
		{
			"Fullscreen",
			"1920x1080",
			"1600x900",
			"1280x720",
			"Enable Game Timer",
			"Enable FPS Counter",
			"Back",
		};

		const DSFLOAT2 positions[amount] =
		{
			{ -0.25f, 0.225f },
			{ -0.25f, 0.075f },
			{ -0.25f, -0.075f },
			{ -0.25f, -0.225f },
			{ 0.25f, 0.225f },
			{ 0.25f, 0.075f },
			{ 0.78f, -0.85f }
		};

		const DSFLOAT2 scales[amount] =
		{
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		void(* const functions[amount])(void*, int) =
		{
			UIFunctions::Settings::SetFullscreen,
			UIFunctions::Settings::SetHighRes,
			UIFunctions::Settings::SetMediumRes,
			UIFunctions::Settings::SetLowRes,
			UIFunctions::Settings::SwitchTimer,
			UIFunctions::Settings::SwitchFPS,
			UIFunctions::Settings::Back,
		};

		const char filenames[amount][32] =
		{
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonMedium"
		};

		const float fontsizes[amount] =
		{
			{ 17.0f },
			{ 17.0f },
			{ 17.0f },
			{ 17.0f },
			{ 16.0f },
			{ 16.0f },
			{ 20.0f }
		};

		for (int i = 0; i < amount; i++)
		{
			auto button = registry.CreateEntity();
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

			uiElement->Setup(filenames[i], texts[i], positions[i], scales[i], fontsizes[i]);

			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
			onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::Image);

			SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
			buttonSound->Load(MENU);
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
			auto button = registry.CreateEntity();
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UISettingsSliderComponent* slider = registry.AddComponent<UISettingsSliderComponent>(button);

			uiElement->Setup("SliderBackground2", texts[i], positions[i]);
			uiElement->AddImage("SliderButton2", positions[i], DSFLOAT2(1.0f, 1.0f), false);
			uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(positions[i].x, positions[i].y + 0.075f));

			float maxLeftPosition = uiElement->m_BaseImage.baseUI.GetPositionBounds().left + 0.11f;
			float maxRightPosition = uiElement->m_BaseImage.baseUI.GetPositionBounds().right - 0.11f;

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

void SettingsState::SetupImages()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsPanel);
	uiElement->Setup("SettingsPanel", "", { 0.0f, 0.0f }, { 1.0f, 1.0f });

}

void SettingsState::SetupText()
{
	
	// Settings Text Header
	auto settingsHeader = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsHeader);
	uiElement->Setup("TempShopTitle", "Settings", { 0.0f, 0.5f }, DSFLOAT2(1.0f, 1.0f), 30.0f);
	uiElement->m_BaseImage.baseUI.SetVisibility(false);
	
}

void SettingsState::Unload()
{
	UnloadEntities();
}