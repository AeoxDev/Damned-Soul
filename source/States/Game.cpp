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
#include "CollisionFunctions.h"
#include "EventFunctions.h"
#include "States\CleanupMacros.h"

void GameScene::Setup(int scene)//Load
{
	RedrawUI();	
	if (scene == 0)
	{
		// Set active
		m_active = true;
		
		//Setup Game HUD
		SetupButtons();
		SetupImages();
		SetupText();

		//Entities
		EntityID dog = registry.CreateEntity();
		EntityID stage = registry.CreateEntity();
		EntityID player = registry.CreateEntity();
		EntityID skeleton = registry.CreateEntity();
		EntityID skeleton2 = registry.CreateEntity();
		EntityID eye = registry.CreateEntity();
		EntityID particle = registry.CreateEntity();

		//Models
		ModelBonelessComponent* dogCo = registry.AddComponent<ModelBonelessComponent>(dog);
		ModelBonelessComponent* stageCo = registry.AddComponent<ModelBonelessComponent>(stage);
		ModelSkeletonComponent* pmc = registry.AddComponent<ModelSkeletonComponent>(player);
		ModelBonelessComponent* skelCo = registry.AddComponent<ModelBonelessComponent>(skeleton);
		ModelBonelessComponent* skelCo2 = registry.AddComponent<ModelBonelessComponent>(skeleton2);
		ModelBonelessComponent* eyeCo = registry.AddComponent<ModelBonelessComponent>(eye);

		//Transforms
		TransformComponent* dtc = registry.AddComponent<TransformComponent>(dog);
		TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
		TransformComponent* ptc = registry.AddComponent<TransformComponent>(player);
		TransformComponent* skeltc = registry.AddComponent<TransformComponent>(skeleton);
		TransformComponent* skeltc2 = registry.AddComponent<TransformComponent>(skeleton2);
		TransformComponent* eyetc = registry.AddComponent<TransformComponent>(eye);

		
		
		SetupPlayerCollisionBox(player, 1.0f);
		
		SetupEnemyCollisionBox(skeleton, 0.9f);
		SetupEnemyCollisionBox(skeleton2, 0.9f);
		SetupEnemyCollisionBox(dog, 1.0f);


		//Stats
		StatComponent* ps = registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		PlayerComponent* pc = registry.AddComponent<PlayerComponent>(player);
		StatComponent* ds = registry.AddComponent<StatComponent>(dog, 100.f, 10.f, 25.f, 5.f);
		StatComponent* ss = registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
		StatComponent* ss2 = registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);
		StatComponent* eyeStats = registry.AddComponent<StatComponent>(eye, 100.0f, 8.0f, 15.0f, 3.0f);

		
		/*AddTimedEventComponentStartContinousEnd(player, player, 1.0f, RandomPosition,
			dog, RandomPosition, 
			player, 2.0f, RandomPosition);*/
			
		//Camera points
		PointOfInterestComponent* poic = registry.AddComponent<PointOfInterestComponent>(player);
		PointOfInterestComponent* dogPoi = registry.AddComponent<PointOfInterestComponent>(dog);
		PointOfInterestComponent* skelPoi = registry.AddComponent<PointOfInterestComponent>(skeleton);
		PointOfInterestComponent* skelPoi2 = registry.AddComponent<PointOfInterestComponent>(skeleton2);

		//ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
		////particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

		UIPlayerHealthComponent* pcUiHpC = registry.AddComponent<UIPlayerHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));
		//Doggo2Ent

		dtc->facingX = 1.0f;
		dogCo->model = LoadModel("HellhoundDummy_PH.mdl");
		skelCo->model = LoadModel("SkeletonOneDymmy.mdl");
		skelCo2->model = LoadModel("SkeletonOneDymmy.mdl");
		stageCo->model = LoadModel("PlaceholderScene.mdl");
		pmc->model = LoadModel("PlayerPlaceholder.mdl");
		eyeCo->model = LoadModel("PlayerPlaceholder.mdl");
		
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
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	CREATE_ENTITY_MAP_entities;

	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<TransformComponent>(registry))
	{
		registry.RemoveComponent<TransformComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ModelSkeletonComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<UIPlayerSoulsComponent>(registry))
	{
		UIPlayerSoulsComponent* ps = registry.GetComponent<UIPlayerSoulsComponent>(entity);
		ps->image.Release();
		registry.RemoveComponent<UIPlayerSoulsComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<UIPlayerHealthComponent>(registry))
	{
		UIPlayerHealthComponent* ph = registry.GetComponent<UIPlayerHealthComponent>(entity);
		ph->image.Release();
		registry.RemoveComponent<UIPlayerHealthComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<PointOfInterestComponent>(registry))
	{
		registry.RemoveComponent<PointOfInterestComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<EnemyComponent>(registry))
	{
		registry.RemoveComponent<EnemyComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<PlayerComponent>(registry))
	{
		registry.RemoveComponent<PlayerComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<StatComponent>(registry))
	{
		registry.RemoveComponent<StatComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ButtonComponent>(registry))
	{
		ButtonComponent* b = registry.GetComponent<ButtonComponent>(entity);
		b->button.Release();
		registry.RemoveComponent<ButtonComponent>(entity);
		
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
		registry.RemoveComponent<TextComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	uint16_t destCount = DestroyEntities(entities);
}