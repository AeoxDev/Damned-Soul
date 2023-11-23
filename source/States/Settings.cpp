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

	const char const texts[amount][32] =
	{
		"\n1280x720",
		"\n1600x900",
		"\n1920x1080",
		"\nFullscreen",
		"\nBack",
		"\nEnable Game Timer"
	};

	const DSFLOAT2 const positions[amount] =
	{
		{ -0.375f, 0.2f },
		{ -0.125f, 0.2f },	
		{ 0.125f, 0.2f },
		{ 0.375f, 0.2f },
		{ -0.81f, -0.8f },
		{ -0.375f, -0.2f },
	};

	const DSFLOAT2 const scales[amount] =
	{
		{ 0.6f, 0.6f },
		{ 0.6f, 0.6f },
		{ 0.6f, 0.6f },
		{ 0.6f, 0.6f },
		{ 0.5f, 0.6f },
		{ 0.6f, 0.6f },
	};

	void(* const functions[amount])(void*, int) =
	{
		UIFunc::Settings_LowRes,
		UIFunc::Settings_MediumRes,
		UIFunc::Settings_HighRes,
		UIFunc::Settings_Fullscreen,
		UIFunc::Settings_Back,
		UIFunc::Settings_Timer,
	};

	for (int i = 0; i < amount; i++)
	{
		auto button = registry.CreateEntity();
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}	

}

void SettingsState::SetupImages()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsPanel);
	uiElement->Setup("ExMenu/ButtonBackgroundHover", "", { 0.0f, 0.0f }, { 2.5f, 2.5f });

}

void SettingsState::SetupText()
{
	
	// Settings Text Header
	auto settingsHeader = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsHeader);
	uiElement->Setup("TempShopTitle", "Settings", { 0.0f, 0.43f }, DSFLOAT2(1.0f, 1.0f), 30.0f);
	uiElement->m_BaseImage.baseUI.SetVisibility(false);
	
}

void SettingsState::Unload()
{
	UnloadEntities();
}