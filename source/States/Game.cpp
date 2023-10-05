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
		EntityID particle = registry.CreateEntity();

		ModelBonelessComponent* dogCo2 = registry.AddComponent<ModelBonelessComponent>(dog2);
		ModelBonelessComponent* stageCo = registry.AddComponent<ModelBonelessComponent>(stage);
		ModelSkeletonComponent* pmc = registry.AddComponent<ModelSkeletonComponent>(player);

		TransformComponent* dtc2 = registry.AddComponent<TransformComponent>(dog2);
		TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
		TransformComponent* ptc = registry.AddComponent<TransformComponent>(player);

		StatComponent* ps = registry.AddComponent<StatComponent>(player, 125, 20.0f, 10, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		PlayerComponent* pc = registry.AddComponent<PlayerComponent>(player);

		PointOfInterestComponent* poic = registry.AddComponent<PointOfInterestComponent>(player);
		PointOfInterestComponent* dogPoi2 = registry.AddComponent<PointOfInterestComponent>(dog2);

		ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
		//particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

		registry.AddComponent<UIPlayerHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));
		//Doggo2Ent

		dogCo2->model.Load("HellhoundDummy_PH.mdl");
		stageCo->model.Load("PlaceholderScene.mdl");
		pmc->model.Load("PlayerPlaceholder.mdl");
		RenderGeometryIndependentCollision(stage);
		poic->mode = POI_ACTIVE;
		poic->weight = 3.f;
		dtc2->positionX = -78.0f;
		dtc2->positionZ = -33.0f;
		dogPoi2->mode = POI_INACTIVE_FOR_X_TIME;
		dogPoi2->weight = 1.75f;
		dogPoi2->time = 15.0f;
		
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
	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<TransformComponent>(registry))
	{
		registry.RemoveComponent<TransformComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ModelSkeletonComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<UIPlayerSoulsComponent>(registry))
	{
		registry.RemoveComponent<UIPlayerSoulsComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<UIHealthComponent>(registry))
	{
		registry.RemoveComponent<UIHealthComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<PointOfInterestComponent>(registry))
	{
		registry.RemoveComponent<PointOfInterestComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<EnemyComponent>(registry))
	{
		registry.RemoveComponent<EnemyComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<PlayerComponent>(registry))
	{
		registry.RemoveComponent<PlayerComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<StatComponent>(registry))
	{
		registry.RemoveComponent<StatComponent>(entity);
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