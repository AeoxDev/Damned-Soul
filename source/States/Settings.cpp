#include "States\Settings.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "UI/UIButtonFunctions.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "UIComponents.h"
#include "Model.h"


void SettingsState::Setup()
{
	m_active = true;

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();

	Camera::ResetCamera();

	//Setup stage to rotate around
	EntityID stage = registry.CreateEntity();
	// Stage Model
	ModelBonelessComponent* stageM = registry.AddComponent<ModelBonelessComponent>(stage);
	stageM->model = LoadModel("PlaceholderScene.mdl");
	// Stage Transform
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	// Stage POI
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);
	stageP->mode = POI_FORCE;
	stageP->height = CAMERA_OFFSET_Y * -0.85f;
	stageP->rotationY = 0.0f;
	stageP->rotationRadius = -0.7f * CAMERA_OFFSET_Z;
	stageP->rotationAccel = 0.12f;
	SetDirectionLight(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
}

void SettingsState::Input()
{
}

void SettingsState::Update()
{
}

void SettingsState::ComputeShaders()
{
}

void SettingsState::SetupButtons()
{
	// Changes resolution to 1280x720	
	auto lowResButton = registry.CreateEntity();
	auto lowResComp = registry.AddComponent<UIButton>(lowResButton);
	lowResComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", "1280x720", UIFunc::Settings_LowRes, { -0.34f, 0.2f }, { 0.55f, 0.55f });

	// Changes resolution to 1600x900
	auto mediumResButton = registry.CreateEntity();
	auto mediumResComp = registry.AddComponent<UIButton>(mediumResButton);
	mediumResComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", "1600x900", UIFunc::Settings_MediumRes, { -0.114f, 0.2f }, { 0.55f, 0.55f });
	
	// Changes resolution to 1920x1080
	auto highResButton = registry.CreateEntity();
	auto highResComp = registry.AddComponent<UIButton>(highResButton);
	highResComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png","1920x1080", UIFunc::Settings_HighRes, { 0.114f, 0.2f }, { 0.55f, 0.55f });

	//Enables/Disables Fullscreen depending on current state
	auto fullscreenButton = registry.CreateEntity();
	auto fullscreenComp = registry.AddComponent<UIButton>(fullscreenButton);
	fullscreenComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", "Fullscreen", UIFunc::Settings_Fullscreen, { 0.34f, 0.2f }, { 0.55f, 0.55f });

	//Back Button
	auto backButton = registry.CreateEntity();
	auto backComp = registry.AddComponent<UIButton>(backButton);
	backComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", "Back", UIFunc::Settings_Back, { -0.81f, -0.8f }, { 0.7f, 0.6f });	
}

void SettingsState::SetupImages()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	auto panelComp = registry.AddComponent<UIImage>(settingsPanel);
	panelComp->Setup("ExMenu/ButtonBackgroundHover.png", { 0.0f, 0.0f }, { 2.5f, 2.5f });

}

void SettingsState::SetupText()
{
	
	// Settings Text Header
	auto settingsHeader = registry.CreateEntity();
	auto headerComp = registry.AddComponent<UIText>(settingsHeader);
	headerComp->Setup("Settings", { 0.0f, 0.43f }, {1.0f, 1.0f});
	
}

void SettingsState::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	UnloadEntities(false);
	
}