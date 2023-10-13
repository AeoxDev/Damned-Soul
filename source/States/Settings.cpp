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


void SettingsState::Setup()
{
	m_active = true;

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();

	Camera::ResetCamera();
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
	lowResComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"1280x720", UIFunc::Settings_LowRes, { -0.4f, 0.2f }, { 1.0f, 1.0f });

	// Changes resolution to 1600x900
	auto mediumResButton = registry.CreateEntity();
	auto mediumResComp = registry.AddComponent<UIButton>(mediumResButton);
	mediumResComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"1600x900", UIFunc::Settings_MediumRes, { -0.4f, 0.0f }, { 1.0f, 1.0f });
	
	// Changes resolution to 1920x1080
	auto highResButton = registry.CreateEntity();
	auto highResComp = registry.AddComponent<UIButton>(highResButton);
	highResComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"1920x1080", UIFunc::Settings_HighRes, { -0.4f, -0.2f }, { 1.0f, 1.0f });

	//Enables/Disables Fullscreen depending on current state
	auto fullscreenButton = registry.CreateEntity();
	auto fullscreenComp = registry.AddComponent<UIButton>(fullscreenButton);
	fullscreenComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"Fullscreen", UIFunc::Settings_Fullscreen, { -0.4f, -0.4f }, { 1.0f, 1.0f });

	//Back Button
	auto backButton = registry.CreateEntity();
	auto backComp = registry.AddComponent<UIButton>(backButton);
	backComp->Setup("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"Back", UIFunc::Settings_Back, { -0.83f, -0.8f }, { 1.0f, 1.0f });	
}

void SettingsState::SetupImages()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	auto panelComp = registry.AddComponent<UIImage>(settingsPanel);
	panelComp->Setup("ExMenu/ButtonBackgroundHover.png", { 0.0f, 0.0f }, { 1.7f, 1.7f });
}

void SettingsState::SetupText()
{
	// Settings Text Header
	auto settingsHeader = registry.CreateEntity();
	auto headerComp = registry.AddComponent<UIText>(settingsHeader);
	headerComp->Setup(L"Settings", { 0.0f, 0.4f }, {1.5f, 1.5f});
}

void SettingsState::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	for (auto entity : View<UIButton>(registry))
	{
		UIButton* b = registry.GetComponent<UIButton>(entity);
		b->Release();
	}

	for (auto entity : View<UIImage>(registry))
	{
		UIImage* i = registry.GetComponent<UIImage>(entity);
		i->Release();
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		registry.DestroyEntity({ i, false });
	}
	
}