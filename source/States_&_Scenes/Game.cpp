#include "States_&_Scenes\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"
#include "AllComponents.h"
#include "Input.h"
#include "States_&_Scenes\StateManager.h"

#include "Menus.h"
#include "UIRenderer.h"

void GameScene::Update()
{

}

void GameScene::Clear()
{
}

void GameScene::Setup(int scene)//Load
{
	
	if (scene == 0)
	{
		//Setup Game HUD
		SetupButtons();
		SetupImages();
		SetupText();


		DrawUi();

		//Doggo
		EntityID dog = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(dog);
		ModelComponent* dogCo = registry.GetComponent<ModelComponent>(dog);
		dogCo->model.Load("HellhoundDummy_PH.mdl");
		/*EntityID stage = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(stage);
		ModelComponent* stageCo = registry.GetComponent<ModelComponent>(stage);
		stageCo->model.Load("PlaceholderScene.mdl");*/
	}
}

void GameScene::SetupButtons()
{

}

void GameScene::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/EmptyHealth.png", { 150.0f, 50.0f }, { 1.0f, 1.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/FullHealth.png", { 150.0f, 50.0f }, { 0.7f, 1.0f }));

}


void GameScene::SetupText()
{

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Current Souls: 0", { 125.0f, 100.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"This is the HUD!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Press ECS to return to main menu!", { sdl.WIDTH / 2.0f - 50.0f, 350.0f }));
}

void GameScene::ComputeShaders()
{
	/*Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();*/
}

void GameScene::Render()
{
	RenderUI();

	//Set shaders here.
	for (auto entity : View<ModelComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		ModelComponent* dogCo = registry.GetComponent<ModelComponent>(entity);
		dogCo->model.RenderAllSubmeshes();
		RenderIndexed(dogCo->model.m_bonelessModel->m_numIndices);
	}
}

void GameScene::Input()
{
	if (keyInput[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInMainMenu(true);
		SetInPlay(false);
		stateManager.menu.Setup();
		Unload();
	}
}

void GameScene::Unload()
{
	for (auto entity : View<ModelComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		ModelComponent* dogCo = registry.GetComponent<ModelComponent>(entity);
		dogCo->model.RenderAllSubmeshes();
		dogCo->model.Free();
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ButtonComponent>(registry))
	{
		registry.GetComponent<ButtonComponent>(entity)->button.Release();
		registry.RemoveComponent<ButtonComponent>(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		registry.GetComponent<ImageComponent>(entity)->image.Release();
		registry.RemoveComponent<ImageComponent>(entity);
	}
}

void GameScene::DrawUi()
{
	ID2D1RenderTarget* rt = ui.GetRenderTarget();

	UpdateUI2();

	Begin2dFrame(ui);

	for (auto entity : View<ButtonComponent>(registry))
		registry.GetComponent<ButtonComponent>(entity)->button.Draw(ui, rt);

	for (auto entity : View<ImageComponent>(registry))

		registry.GetComponent<ImageComponent>(entity)->image.Draw(rt);

	for (auto entity : View<TextComponent>(registry))
		registry.GetComponent<TextComponent>(entity)->text.Draw(ui);

	End2dFrame(ui);
}