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
	texts[0] = "\n1280x720";
	texts[1] = "\n1600x900";
	texts[2] = "\n1920x1080";
	texts[3] = "\nFullscreen";
	texts[4] = "\nBack";

	ML_Array<DSFLOAT2, 5> positions;
	positions[0] = { -0.375f, 0.2f };
	positions[1] = { -0.125f, 0.2f };
	positions[2] = { 0.125f, 0.2f };
	positions[3] = { 0.375f, 0.2f };
	positions[4] = { -0.81f, -0.8f };

	ML_Array<DSFLOAT2, 5> scales;
	scales[0] = { 0.6f, 0.6f };
	scales[1] = { 0.6f, 0.6f };
	scales[2] = { 0.6f, 0.6f };
	scales[3] = { 0.6f, 0.6f };
	scales[4] = { 0.5f, 0.6f };

	ML_Array<void(*)(void*, int), 5> functions;
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

		uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

	}	

	auto settingsPanel = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsPanel);
	uiElement->Setup("ExMenu/ButtonBackgroundHover", "", { 0.0f, 0.0f }, { 2.5f, 2.5f });

}

void SettingsState::SetupImages()
{
	// Settings backdrop panel

}

void SettingsState::SetupText()
{
	
	// Settings Text Header
	auto settingsHeader = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(settingsHeader);
	uiElement->Setup("TempShopTitle", "Settings", { 0.0f, 0.43f });
	uiElement->m_BaseImage.baseUI.SetVisibility(false);
	
}

void SettingsState::Unload()
{
	UnloadEntities();
}