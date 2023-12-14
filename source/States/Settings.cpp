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
#include "DeltaTime.h"

void SettingsState::Setup()
{

	RedrawUI();
	SetupUI();

	Camera::ResetCamera();
	Camera::SetCutsceneMode(0);

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
	gameSpeed = 1.0f;
}

void SettingsState::Input()
{

}

void SettingsState::SetupUI()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	UIComponent* uiElementP = registry.AddComponent<UIComponent>(settingsPanel);
	uiElementP->Setup("SettingsPanel", "", "Settings", { 0.0f, 0.0f }, { 1.0f, 1.0f }, 30.0f);
	uiElementP->m_BaseText.baseUI.SetPosition({ 0.0f, 0.5f });

	const int amount = 6;
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
			"Back",
		};

		const DSFLOAT2 positions[amount] =
		{
			{ -0.25f, 0.225f },
			{ -0.25f, 0.075f },
			{ -0.25f, -0.075f },
			{ -0.25f, -0.225f },
			{ 0.25f, 0.225f },
			{ 0.78f, -0.85f }
		};

		const DSFLOAT2 scales[amount] =
		{
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
			UIFunctions::Settings::Back,
		};

		const char filenames[amount][32] =
		{
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonSmall",
			"ButtonMedium"
		};

		const char filenamesHover[amount][32] =
		{
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonSmallHover",
			"ButtonMediumHover"
		};

		const float fontsizes[amount] =
		{
			{ 17.0f },
			{ 17.0f },
			{ 17.0f },
			{ 17.0f },
			{ 16.0f },
			{ 20.0f }
		};

		for (int i = 0; i < amount; i++)
		{
			auto button = registry.CreateEntity();
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

			if (i == 4 && GetVisualTimer())
			{
				uiElement->Setup(filenamesHover[i], filenames[i], texts[i], positions[i], scales[i], fontsizes[i]);
			}
			else
			{
				uiElement->Setup(filenames[i], filenamesHover[i], texts[i], positions[i], scales[i], fontsizes[i]);
			}

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

			uiElement->Setup("Slider/BorderSmallSlider", texts[i], positions[i]);
			uiElement->AddImage("Slider/SliderButton75", positions[i], DSFLOAT2(1.0f, 1.0f), false);
			uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(positions[i].x, positions[i].y + 0.075f));

			float maxLeftPosition = (uiElement->m_BaseImage.baseUI.GetPositionBounds().left / 2.0f) + 0.025f;
			float maxRightPosition = (uiElement->m_BaseImage.baseUI.GetPositionBounds().right / 2.0f) - 0.025f;

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

void SettingsState::Unload()
{
	UnloadEntities();
}