#include "States\Menu.h"
#include "UI/UIRenderer.h"
#include "Hitbox.h"
#include "States\StateManager.h"
#include "Input.h"
#include "Registry.h"
#include "Components.h"
#include "Camera.h"

void Menu::Setup()//Load
{
	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();
	//Setup stage to rotate around
	EntityID stage = registry.CreateEntity();
	ModelBonelessComponent* stageM = registry.AddComponent<ModelBonelessComponent>(stage);
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);

	stageM->model.Load("PlaceholderScene.mdl");
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
		auto OnClick = [this]()
			{
				SetInPlay(true);
				SetInMainMenu(false);
				Unload();
				stateManager.levelScenes[0].Setup(0);
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/StartButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.4f }));
	}

	//Options Button
	{
		auto OnClick = [this]()
			{
				SetInSettings(true);
				SetInMainMenu(false);
				Unload();
				stateManager.settings.Setup();
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/OptionsButton.png", "", L"", OnClick, OnHover, { 0.0f,  -0.6f}));
	}

	//Exit Button
	{
		auto OnClick = [this]()
			{
				
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.8f }));
	}
}

void Menu::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/ExTitle.png", { 0.0f, 0.6f }, { 2.0f, 2.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/Eye.png", { -0.8f, 0.6f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/Eye.png", { 0.8f, 0.6f }, { 1.5f, 1.5f }));
}

void Menu::SetupText()
{
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"This is the main menu!", { 0.0f, 0.0f }));
}

void Menu::Unload()
{
	for (auto entity : View<ButtonComponent>(registry))
	{
		registry.RemoveComponent<ButtonComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		registry.RemoveComponent<ImageComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<TextComponent>(registry))
	{
		registry.RemoveComponent<TextComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<PointOfInterestComponent>(registry))
	{
		registry.RemoveComponent<PointOfInterestComponent>(entity);
		ModelBonelessComponent* m = registry.GetComponent<ModelBonelessComponent>(entity);
		m->model.Free();
		registry.RemoveComponent<TransformComponent>(entity);
		registry.DestroyEntity(entity);
	}

	ClearUI();
}