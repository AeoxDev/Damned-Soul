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

void Menu::Setup()//Load
{
	m_active = true;
	// Clear relics when entering the main menu
	Relics::ClearRelicFunctions();

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
	stateManager.activeLevelScene = 0;
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

void Menu::Input()
{
	
	//Particles::PrepareParticleCompute();
	//Dispatch(1, 2, 0);
	//Particles::FinishParticleCompute();
}

void Menu::Update()
{

}

void Menu::ComputeShaders()
{

}

void Menu::SetupButtons()
{
	//Start Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", L"Start", UIFunc::MainMenu_Start, { -0.81f, -0.28f }, {0.7f, 0.6f} );
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}

	//Settings Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", L"Settings", UIFunc::MainMenu_Settings, { -0.81f,  -0.54f }, {0.7f, 0.6f} );
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}

	//Exit Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", L"Quit", UIFunc::MainMenu_Quit, { -0.81f, -0.8f }, {0.7f, 0.6f} );
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}
}

void Menu::SetupImages()
{
	//Title
	auto title = registry.CreateEntity();
	auto tc = registry.AddComponent<UIImage>(title);
	tc->Setup("ExMenu/DamnedTitle3.png", { 0.0f, 0.20f }, { 1.0f, 1.0f});
	/*
	auto title2 = registry.CreateEntity();
	auto tc2 = registry.AddComponent<UIImage>(title2);
	tc2->Setup("ExMenu/DamnedTitle2.png", { 0.0f, 0.0f }, { 1.0f, 1.0f });

	auto title3 = registry.CreateEntity();
	auto tc3 = registry.AddComponent<UIImage>(title3);
	tc3->Setup("ExMenu/DamnedTitle3.png", { 0.0f, -0.25f }, { 1.0f, 1.0f });
	*/
/*
	//Eye 1
	auto eye1 = registry.CreateEntity();
	auto ec1 = registry.AddComponent<UIImage>(eye1);
	ec1->Setup("ExMenu/Eye.png", { -0.8f, 0.6f }, { 1.5f, 1.5f });

	//Eye 2
	auto eye2 = registry.CreateEntity();
	auto ec2 = registry.AddComponent<UIImage>(eye2);
	ec2->Setup("ExMenu/Eye.png", { 0.8f, 0.6f }, { 1.5f, 1.5f });
*/
}

void Menu::SetupText()
{
	/*
	auto t1 = registry.CreateEntity();
	auto tc1 = registry.AddComponent<UIText>(t1);
	tc1->Setup(L"This is the main menu!", { 0.0f, 0.0f });
	*/
}

void Menu::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	UnloadEntities(false);

	ClearUI();
}