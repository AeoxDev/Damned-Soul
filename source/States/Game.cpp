#include "States\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"
#include "Components.h"
#include "Input.h"
#include "States\StateManager.h"
#include "Registry.h"
#include "UIRenderer.h"

void GameScene::Setup(int scene)//Load
{
	RedrawUI();	
	if (scene == 0)
	{
		
		//Setup Game HUD
		SetupButtons();
		SetupImages();
		SetupText();

		//Doggo
		EntityID dog2 = registry.CreateEntity();
		EntityID stage = registry.CreateEntity();
		EntityID player = registry.CreateEntity();

		ModelComponent* dogCo2 = registry.AddComponent<ModelComponent>(dog2);
		ModelComponent* stageCo = registry.AddComponent<ModelComponent>(stage);
		ModelComponent* pmc = registry.AddComponent<ModelComponent>(player);

		TransformComponent* dtc2 = registry.AddComponent<TransformComponent>(dog2);
		TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
		TransformComponent* ptc = registry.AddComponent<TransformComponent>(player);

		PlayerComponent* pc = registry.AddComponent<PlayerComponent>(player);

		PointOfInterestComponent* poic = registry.AddComponent<PointOfInterestComponent>(player);
		PointOfInterestComponent* dogPoi2 = registry.AddComponent<PointOfInterestComponent>(dog2);

		registry.AddComponent<UIPlayerHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));
		//Doggo2Ent

		dogCo2->model.Load("HellhoundDummy_PH.mdl");
		stageCo->model.Load("PlaceholderScene.mdl");
		pmc->model.Load("HellhoundDummy_PH.mdl");
		RenderGeometryIndependentCollision(stage);
		poic->mode = POI_ACTIVE;
		poic->weight = 2.f;
		dtc2->positionX = -80.0f;
		dtc2->positionZ = 33.0f;
		
		//portPoi->SetPOImode(POI_FORCE);
	}
}

void GameScene::Input()
{
	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInMainMenu(true);
		SetInPlay(false);
		Unload();
		stateManager.menu.Setup();
	}
}

void GameScene::Update()
{
	
}

void GameScene::ComputeShaders()
{
	/*Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();*/
}

void GameScene::SetupButtons()
{

}

void GameScene::SetupImages()
{
	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/FullHealth.png", { -0.9f, 0.8f }, { 0.7f, 1.0f }));

	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/EmptyHealth.png", { -0.8f, 0.8f }, { 1.0f, 1.0f }));
}

void GameScene::SetupText()
{

	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Current Souls: 0", { -0.8f, 0.6f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"This is the HUD!", { 0.0f, 0.6f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Press ECS to return to main menu!", { 0.0f, 0.4f }));
}

void GameScene::Unload()
{
	for (auto entity : View<ModelComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelComponent* dogCo = registry.GetComponent<ModelComponent>(entity);
		dogCo->model.RenderAllSubmeshes();
		dogCo->model.Free();
		registry.DestroyEntity(entity);
	}

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
}