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
	
	stageVars.stageNr = rand()% 5;
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
	const int amount = 6;
	const int sliderAmount = 5;

	{
		const char const texts[amount][32] =
		{
			"Fullscreen",
			"1920x1080",
			"1600x900",
			"1280x720",
			"Enable Game Timer",
			"Back",
		};

		const DSFLOAT2 const positions[amount] =
		{
			{ -0.4f, 0.3f },
			{ -0.4f, 0.2f },
			{ -0.4f, 0.1f },
			{ -0.4f, 0.0f },
			{ -0.375f, -0.2f },
			{ -0.5f, -0.45f },
		};

		const DSFLOAT2 const scales[amount] =
		{
			{ 0.5f, 0.15f },
			{ 0.5f, 0.15f },
			{ 0.5f, 0.15f },
			{ 0.5f, 0.15f },
			{ 0.3f, 0.3f },
			{ 0.2f, 0.2f },
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

		for (int i = 0; i < amount; i++)
		{
			auto button = registry.CreateEntity();
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

			uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
			onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::Image);

			SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
			buttonSound->Load(MENU);
		}
	}

	{
		const char const texts[sliderAmount][32] =
		{
			"Master Volume",
			"Voice Volume",
			"SFX Volume",
			"Music Volume",
			"Ambient Volume",
		};

		const DSFLOAT2 const positions[sliderAmount] =
		{
			{ 0.4f, 0.3f },
			{ 0.4f, 0.1f },
			{ 0.4f, -0.1f },
			{ 0.4f, -0.3f },
			{ 0.4f, -0.5f },
		};

		void(* const functions[sliderAmount * 2])(void*, int) =
		{
			UIFunctions::Settings::Volume::Master_Press,
			UIFunctions::Settings::Volume::Master_Release,
			UIFunctions::Settings::Volume::Voice_Press,
			UIFunctions::Settings::Volume::Voice_Release,
			UIFunctions::Settings::Volume::SFX_Press,
			UIFunctions::Settings::Volume::SFX_Release,
			UIFunctions::Settings::Volume::Music_Press,
			UIFunctions::Settings::Volume::Music_Release,
			UIFunctions::Settings::Volume::Ambient_Press,
			UIFunctions::Settings::Volume::Ambient_Release,
		};

		for (int i = 0; i < 1; i++)
		{
			auto button = registry.CreateEntity();
			OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
			UIComponent* uiElement = registry.AddComponent<UIComponent>(button);
			UISettingsSliderComponent* slider = registry.AddComponent<UISettingsSliderComponent>(button);

			uiElement->Setup("SliderBackground2", texts[i], positions[i]);
			uiElement->AddImage("SliderButton2", positions[i], DSFLOAT2(1.0f, 1.0f), false);
			uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(positions[i].x, positions[i].y + 0.075f));
			slider->currentPosition = uiElement->m_Images[0].baseUI.GetPosition().x;

			onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], functions[i + 1]);

		}
	}
}

void SettingsState::SetupImages()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsPanel);
	uiElement->Setup("ExMenu/ButtonBackgroundHover", "", { 0.0f, 0.0f }, { 3.0f, 3.0f });

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