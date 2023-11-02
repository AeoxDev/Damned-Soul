#include "States\Menu.h"
#include "UI/UIRenderer.h"
#include "Hitbox.h"
#include "States\StateManager.h"
#include "Input.h"
#include "Registry.h"
#include "Components.h"
#include "Camera.h"
#include "States\CleanupMacros.h"
#include "UI/UIButtonFunctions.h"
#include "Level.h"
#include "Model.h"
#include "UIComponents.h"
#include "RelicFunctions.h"

#include "SDLHandler.h"

void Menu::Setup()//Load
{
	// Clear relics when entering the main menu
	Relics::ClearRelicFunctions();

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

	stateManager.activeLevel = 0;
}

void Menu::Input()
{
	
}

void Menu::SetupButtons()
{

	ML_Array<ML_String, 3> texts;
	texts[0] = "\nStart";
	texts[1] = "\nSettings";
	texts[2] = "\nQuit";

	ML_Array<DSFLOAT2, 3> positions;
	positions[0] = { -0.81f, -0.28f };
	positions[1] = { -0.81f,  -0.54f };
	positions[2] = { -0.81f, -0.8f };

	ML_Array<DSFLOAT2, 3> scales;
	scales[0] = { 0.7f, 0.6f };
	scales[1] = { 0.7f, 0.6f };
	scales[2] = { 0.7f, 0.6f };

	ML_Array<void(*)(void*, int), 3> functions;
	functions[0] = UIFunc::LoadNextLevel;
	functions[1] = UIFunc::MainMenu_Settings;
	functions[2] = UIFunc::MainMenu_Quit;

	for (int i = 0; i < 3; i++)
	{
		auto button = registry.CreateEntity();
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}
}

void Menu::SetupImages()
{
	//Title
	auto title = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(title);
	uiElement->Setup("ExMenu/DamnedTitle3", "", { 0.0f, 0.20f });
}

void Menu::SetupText()
{

}

void Menu::Unload()
{
	UnloadEntities();
	ClearUI();
}