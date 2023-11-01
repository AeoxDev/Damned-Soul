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

void SettingsState::SetupButtons()
{
	ML_Array<ML_String, 5> texts;
	texts[0] = "1280x720";
	texts[1] = "1600x900";
	texts[2] = "1920x1080";
	texts[3] = "Fullscreen";
	texts[4] = "Back";

	ML_Array<DSFLOAT2, 5> positions;
	positions[0] = { -0.34f, 0.2f };
	positions[1] = { -0.114f, 0.2f };
	positions[2] = { 0.114f, 0.2f };
	positions[3] = { 0.34f, 0.2f };
	positions[4] = { -0.81f, -0.8f };

	ML_Array<DSFLOAT2, 5> scales;
	scales[0] = { 0.55f, 0.55f };
	scales[1] = { 0.55f, 0.55f };
	scales[2] = { 0.55f, 0.55f };
	scales[3] = { 0.55f, 0.55f };
	scales[4] = { 0.7f, 0.6f };

	ML_Array<void(*)(void*), 5> functions;
	functions[0] = UIFunc::Settings_LowRes;
	functions[1] = UIFunc::Settings_MediumRes;
	functions[2] = UIFunc::Settings_HighRes;
	functions[3] = UIFunc::Settings_Fullscreen;
	functions[4] = UIFunc::Settings_Back;

	for (int i = 0; i < 5; i++)
	{
		auto button = registry.CreateEntity();
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", "Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);
	}	
}

void SettingsState::SetupImages()
{
	// Settings backdrop panel
	auto settingsPanel = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsPanel);
	uiElement->Setup("ExMenu/ButtonBackgroundHover", "", "", { 0.0f, 0.0f }, { 2.5f, 2.5f });

}

void SettingsState::SetupText()
{
	
	// Settings Text Header
	auto settingsHeader = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsHeader);
	uiElement->Setup("", "", "Settings", { 0.0f, 0.43f });
	
}

void SettingsState::Unload()
{
	UnloadEntities();
}