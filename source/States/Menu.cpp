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

void Menu::Setup()//Load
{
	m_active = true;

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
	stateManager.activeLevelScene = 0;
	Camera::ResetCamera();

	//Setup stage to rotate around
	EntityID stage = registry.CreateEntity();
	ModelBonelessComponent* stageM = registry.AddComponent<ModelBonelessComponent>(stage);
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);

	stageM->model = LoadModel("PlaceholderScene.mdl");
	stageP->mode = POI_FORCE;
	stageP->height = CAMERA_OFFSET_Y * -0.85f;
	stageP->rotationY = 0.0f;
	stageP->rotationRadius = -0.7f * CAMERA_OFFSET_Z;
	stageP->rotationAccel = 0.12f;
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
		comp->Setup("Exmenu/StartButton.png", "Exmenu/StartButtonHover.png", L"", UIFunc::MainMenu_Start, { 0.0f, -0.4f });
	}

	//Settings Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/OptionsButton.png", "Exmenu/OptionsButtonHover.png", L"", UIFunc::MainMenu_Settings, { 0.0f,  -0.6f });
	}

	//Exit Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ExitButton.png", "Exmenu/ExitButtonHover.png", L"", UIFunc::MainMenu_Quit, { 0.0f, -0.8f });
	}
}

void Menu::SetupImages()
{
	//Title
	auto title = registry.CreateEntity();
	auto tc = registry.AddComponent<UIImage>(title);
	tc->Setup("ExMenu/ExTitle.png", { 0.0f, 0.6f }, { 2.0f, 2.0f });

	//Eye 1
	auto eye1 = registry.CreateEntity();
	auto ec1 = registry.AddComponent<UIImage>(eye1);
	ec1->Setup("ExMenu/Eye.png", { -0.8f, 0.6f }, { 1.5f, 1.5f });

	//Eye 2
	auto eye2 = registry.CreateEntity();
	auto ec2 = registry.AddComponent<UIImage>(eye2);
	ec2->Setup("ExMenu/Eye.png", { 0.8f, 0.6f }, { 1.5f, 1.5f });
}

void Menu::SetupText()
{
	auto t1 = registry.CreateEntity();
	auto tc1 = registry.AddComponent<UIText>(t1);
	tc1->Setup(L"This is the main menu!", { 0.0f, 0.0f });
}

void Menu::Unload()
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

	for (auto entity : View<ModelBonelessComponent>(registry))
	{
		ModelBonelessComponent* m = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(m->model);
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* m = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(m->model);
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		registry.DestroyEntity({ i, false });
	}

	ClearUI();
}