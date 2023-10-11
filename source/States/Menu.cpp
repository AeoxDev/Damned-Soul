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

void Menu::Setup()//Load
{
	m_active = true;

	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();

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
		//auto OnClick = [this]()
		//	{
		//		SetInPlay(true);
		//		SetInMainMenu(false);
		//		Unload();
		//		stateManager.levelScenes[0].Setup(0);
		//	};

		//auto OnHover = [this]()
		//	{

		//	};

		UIButtonComponent* button = registry.AddComponent<UIButtonComponent>(registry.CreateEntity());
		button->button.Setup("Exmenu/StartButton.png", "Exmenu/StartButtonHover.png", L"", UIFunc::MainMenu_Start, /*OnClick, OnHover,*/ { 0.0f, -0.4f });
	}

	//Options Button
	{
		//auto OnClick = [this]()
		//	{
		//		SetInSettings(true);
		//		SetInMainMenu(false);
		//		Unload();
		//		stateManager.settings.Setup();
		//	};

		//auto OnHover = [this]()
		//	{

		//	};

		UIButtonComponent* button = registry.AddComponent<UIButtonComponent>(registry.CreateEntity());
		button->button.Setup("Exmenu/OptionsButton.png", "Exmenu/OptionsButtonHover.png", L"", UIFunc::MainMenu_Settings, /*OnClick, OnHover,*/ { 0.0f,  -0.6f });
	}

	//Exit Button
	{
		//auto OnClick = [this]()
		//	{
		//		sdl.quit = true;
		//	};

		//auto OnHover = [this]()
		//	{

		//	};

		UIButtonComponent* button = registry.AddComponent<UIButtonComponent>(registry.CreateEntity());
		button->button.Setup("Exmenu/ExitButton.png", "Exmenu/ExitButtonHover.png", L"", UIFunc::MainMenu_Quit, /*OnClick, OnHover, */{ 0.0f, -0.8f });
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
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	CREATE_ENTITY_MAP_entities;

	for (auto entity : View<UIButtonComponent>(registry))
	{
		UIButtonComponent* b = registry.GetComponent<UIButtonComponent>(entity);
		b->button.Release();
		registry.RemoveComponent<UIButtonComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		ImageComponent* i = registry.GetComponent<ImageComponent>(entity);
		i->image.Release();
		registry.RemoveComponent<ImageComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<TextComponent>(registry))
	{
		// Text doesn't need to be released
		registry.RemoveComponent<TextComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<PointOfInterestComponent>(registry))
	{
		registry.RemoveComponent<PointOfInterestComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ModelBonelessComponent>(registry))
	{
		ModelBonelessComponent* m = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(m->model);
		registry.RemoveComponent<TransformComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* m = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(m->model);
		registry.RemoveComponent<TransformComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	uint16_t destCount = DestroyEntities(entities);

	ClearUI();
}