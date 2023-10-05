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
#include "UI/UIRenderer.h"

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
		EntityID dog = registry.CreateEntity();
		EntityID stage = registry.CreateEntity();
		EntityID player = registry.CreateEntity();
		EntityID skeleton = registry.CreateEntity();
		EntityID skeleton2 = registry.CreateEntity();
		EntityID particle = registry.CreateEntity();

		ModelBonelessComponent* dogCo = registry.AddComponent<ModelBonelessComponent>(dog);
		ModelBonelessComponent* stageCo = registry.AddComponent<ModelBonelessComponent>(stage);
		ModelSkeletonComponent* pmc = registry.AddComponent<ModelSkeletonComponent>(player);
		ModelBonelessComponent* skelCo = registry.AddComponent<ModelBonelessComponent>(skeleton);
		ModelBonelessComponent* skelCo2 = registry.AddComponent<ModelBonelessComponent>(skeleton2);

		TransformComponent* dtc = registry.AddComponent<TransformComponent>(dog);
		TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
		TransformComponent* ptc = registry.AddComponent<TransformComponent>(player);
		TransformComponent* skeltc = registry.AddComponent<TransformComponent>(skeleton);
		TransformComponent* skeltc2 = registry.AddComponent<TransformComponent>(skeleton2);

		StatComponent* ps = registry.AddComponent<StatComponent>(player, 125, 20.0f, 10, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		PlayerComponent* pc = registry.AddComponent<PlayerComponent>(player, 4);

		StatComponent* ps = registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		PlayerComponent* pc = registry.AddComponent<PlayerComponent>(player);
		StatComponent* ds = registry.AddComponent<StatComponent>(dog, 100.f, 10.f, 25.f, 5.f);
		StatComponent* ss = registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
		StatComponent* ss2 = registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);



		PointOfInterestComponent* poic = registry.AddComponent<PointOfInterestComponent>(player);
		PointOfInterestComponent* dogPoi = registry.AddComponent<PointOfInterestComponent>(dog);
		PointOfInterestComponent* skelPoi = registry.AddComponent<PointOfInterestComponent>(skeleton);
		PointOfInterestComponent* skelPoi2 = registry.AddComponent<PointOfInterestComponent>(skeleton2);

		registry.AddComponent<UIHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		registry.AddComponent<UIPlayerSoulsComponent>(player, 1, DirectX::XMFLOAT2(-0.8f, 0.6f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));

		//registry.AddComponent<UIHealthComponent>(dog2, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), UIImage("ExMenu/FullHealth.png"), UIText(L""));

		ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
		//particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

		registry.AddComponent<UIPlayerHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));
		//Doggo2Ent

		dtc->facingX = 1.0f;
		dogCo->model.Load("HellhoundDummy_PH.mdl");
		skelCo->model.Load("SkeletonOneDymmy.mdl");
		skelCo2->model.Load("SkeletonOneDymmy.mdl");
		stageCo->model.Load("PlaceholderScene.mdl");
		pmc->model.Load("PlayerPlaceholder.mdl");
		RenderGeometryIndependentCollision(stage);
		//poic->active = POI_ACTIVE;
		dtc->positionX = 20.0f;
		skeltc->positionZ = 20.0f;
		skeltc2->positionZ = 15.0f;

		HellhoundBehaviour* hellhoundBehevCo = registry.AddComponent<HellhoundBehaviour>(dog);
		SkeletonBehaviour* skeletonBehevCo = registry.AddComponent<SkeletonBehaviour>(skeleton);
		SkeletonBehaviour* skeletonBehevCo2 = registry.AddComponent<SkeletonBehaviour>(skeleton2);
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
		dogCo->model.RenderAllSubmeshes();
		dogCo->model.Free();
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ModelSkeletonComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
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