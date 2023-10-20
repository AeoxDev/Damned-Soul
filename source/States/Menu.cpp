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

#include "SDLHandler.h"

void Menu::Setup()//Load
{
	m_active = true;

	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();
	Camera::ResetCamera();


	//Temp stuff for ui to not crash because saving between levels is not fully implemented
	EntityID playerUi = registry.CreateEntity();
	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));

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

	stateManager.activeLevel = 0;
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
		comp->Setup("Exmenu/StartButton.png", "Exmenu/StartButtonHover.png", "", UIFunc::LoadNextLevel, { 0.0f, -0.4f });
	}

	//Settings Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/OptionsButton.png", "Exmenu/OptionsButtonHover.png", "", UIFunc::MainMenu_Settings, { 0.0f,  -0.6f });
	}

	//Exit Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ExitButton.png", "Exmenu/ExitButtonHover.png", "", UIFunc::MainMenu_Quit, { 0.0f, -0.8f });
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

	for (auto entity : View<UIRelicComponent>(registry))
	{
		UIRelicComponent* r = registry.GetComponent<UIRelicComponent>(entity);
		r->sprite.Release();
		r->flavorImage.Release();
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		EntityID check = registry.entities.at(i).id;
		if (check.state == false)
			registry.DestroyEntity(check);
	}

	ClearUI();
}