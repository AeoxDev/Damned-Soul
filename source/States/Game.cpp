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
#include "Camera.h"

#include "MemLib\ML_String.hpp"
#include "MemLib\ML_Vector.hpp"


void GameScene::Setup(int scene)//Load
{
	RedrawUI();	

	ML_String valueAsString = std::to_string(scene).c_str();
	std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());

	if (scene == 0)
	{
		// Set active
		m_active = true;
		
		//Setup Game HUD
		SetupButtons();
		SetupImages();
		SetupText();

		Camera::ResetCamera();

		//Doggo
		EntityID gameLevel = registry.CreateEntity();
		EntityID dog = registry.CreateEntity();
		EntityID stage = registry.CreateEntity();
		EntityID player = registry.CreateEntity();
		EntityID skeleton = registry.CreateEntity();
		EntityID skeleton2 = registry.CreateEntity();
		EntityID particle = registry.CreateEntity();

		ModelBonelessComponent* dogCo = registry.AddComponent<ModelBonelessComponent>(dog);
		ModelBonelessComponent* stageCo = registry.AddComponent<ModelBonelessComponent>(stage);
		ModelSkeletonComponent* pmc = registry.AddComponent<ModelSkeletonComponent>(player);
		AnimationComponent* pac = registry.AddComponent<AnimationComponent>(player);
		ModelBonelessComponent* skelCo = registry.AddComponent<ModelBonelessComponent>(skeleton);
		ModelBonelessComponent* skelCo2 = registry.AddComponent<ModelBonelessComponent>(skeleton2);

		TransformComponent* dtc = registry.AddComponent<TransformComponent>(dog);
		TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
		TransformComponent* ptc = registry.AddComponent<TransformComponent>(player);
		TransformComponent* skeltc = registry.AddComponent<TransformComponent>(skeleton);
		TransformComponent* skeltc2 = registry.AddComponent<TransformComponent>(skeleton2);



		StatComponent* ps = registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		PlayerComponent* pc = registry.AddComponent<PlayerComponent>(player);
		RelicHolderComponent* pRhc= registry.AddComponent<RelicHolderComponent>(player, "Relic Holder");

		StatComponent* ds = registry.AddComponent<StatComponent>(dog, 50.f, 20.f, 25.f, 5.f);
		EnemyComponent* ec1 = registry.AddComponent<EnemyComponent>(dog, 1);

		StatComponent* ss = registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
		EnemyComponent* ec2 = registry.AddComponent<EnemyComponent>(skeleton, 2);

		StatComponent* ss2 = registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);
		EnemyComponent* ec3 = registry.AddComponent<EnemyComponent>(skeleton2, 2);

		/*AddTimedEventComponentStartContinousEnd(player, player, 1.0f, RandomPosition,
			dog, RandomPosition, 
			player, 2.0f, RandomPosition);*/
		ControllerComponent* cc = registry.AddComponent<ControllerComponent>(player);

		

		PointOfInterestComponent* poic = registry.AddComponent<PointOfInterestComponent>(player);
		PointOfInterestComponent* dogPoi = registry.AddComponent<PointOfInterestComponent>(dog);
		PointOfInterestComponent* skelPoi = registry.AddComponent<PointOfInterestComponent>(skeleton);
		PointOfInterestComponent* skelPoi2 = registry.AddComponent<PointOfInterestComponent>(skeleton2);

		ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);

		UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));
		
		ML_Vector<UIImage> uiRelics;
		//UIPlayerRelicsComponent* pcUiRC = registry.AddComponent<UIPlayerRelicsComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.4f), DirectX::XMFLOAT2(1.0f, 0.5f), UIImage("TempRelicHolder.png"), uiRelics, 0);

		UIHealthComponent* dogUIHpc = registry.AddComponent<UIHealthComponent>(dog, 1.0f, DirectX::XMFLOAT2(0.8f, 0.8f), DirectX::XMFLOAT2(0.6f, 0.6f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		UIHealthComponent* skelUIHpC1 = registry.AddComponent<UIHealthComponent>(skeleton, 1.0f, DirectX::XMFLOAT2(0.8f, 0.6f), DirectX::XMFLOAT2(0.6f, 0.6f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		UIHealthComponent* skelUIHpC2 = registry.AddComponent<UIHealthComponent>(skeleton2, 1.0f, DirectX::XMFLOAT2(0.8f, 0.4f), DirectX::XMFLOAT2(0.6f, 0.6f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		
		UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, 1, DirectX::XMFLOAT2(0.9f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/CheckboxBase.png"), UIText(valueAsWString));

		//Doggo2Ent

		dtc->facingX = 1.0f;
		dogCo->model = LoadModel("HellhoundDummy_PH.mdl");
		skelCo->model = LoadModel("SkeletonOneDymmy.mdl");
		skelCo2->model = LoadModel("SkeletonOneDymmy.mdl");
		stageCo->model = LoadModel("PlaceholderScene.mdl");
		pmc->model = LoadModel("PlayerPlaceholder.mdl");
		(*pac) = AnimationComponent();

		
		RenderGeometryIndependentCollision(stage);
		//poic->active = POI_ACTIVE;
		dtc->positionX = 20.0f;
		skeltc->positionZ = 20.0f;
		skeltc2->positionZ = 15.0f;
		poic->weight = 10.0f;

		HellhoundBehaviour* hellhoundBehevCo = registry.AddComponent<HellhoundBehaviour>(dog);
		SkeletonBehaviour* skeletonBehevCo = registry.AddComponent<SkeletonBehaviour>(skeleton);
		SkeletonBehaviour* skeletonBehevCo2 = registry.AddComponent<SkeletonBehaviour>(skeleton2);

		//Finally set the collision boxes
		SetupPlayerCollisionBox(player, 1.0f);

		SetupEnemyCollisionBox(skeleton, 0.9f);
		SetupEnemyCollisionBox(skeleton2, 0.9f);
		SetupEnemyCollisionBox(dog, 1.0f);
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

	if (keyState[SDL_SCANCODE_1] == pressed)
	{
		for (auto entity : View<RelicHolderComponent, UIPlayerRelicsComponent>(registry))
		{
			auto relicHolder = registry.GetComponent<RelicHolderComponent>(entity);
			auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);

			relicHolder->AddRelic<DamageRelic>();

			UIImage tempImage("TempRelic1.png");
			uiElement->images.push_back(tempImage);
		}
	}
	if (keyState[SDL_SCANCODE_2] == pressed)
	{
		for (auto entity : View<RelicHolderComponent, UIPlayerRelicsComponent>(registry))
		{
			auto relicHolder = registry.GetComponent<RelicHolderComponent>(entity);
			auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);

			relicHolder->AddRelic<SpeedRelic>();

			UIImage tempImage("TempRelic2.png");
			uiElement->images.push_back(tempImage);
		}
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

}

void GameScene::SetupText()
{

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

	for (auto entity : View<UIGameLevelComponent>(registry))
	{
		UIGameLevelComponent* ps = registry.GetComponent<UIGameLevelComponent>(entity);
		ps->image.Release();
		registry.RemoveComponent<UIGameLevelComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<UIHealthComponent>(registry))
	{
		UIHealthComponent* ph = registry.GetComponent<UIHealthComponent>(entity);
		ph->image.Release();
		registry.RemoveComponent<UIHealthComponent>(entity);
		
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

	for (auto entity : View<RelicHolderComponent>(registry))
	{
		registry.RemoveComponent<RelicHolderComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<StatComponent>(registry))
	{
		registry.RemoveComponent<StatComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

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
		registry.RemoveComponent<TextComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<UIPlayerRelicsComponent>(registry))
	{
		UIPlayerRelicsComponent* uprc = registry.GetComponent<UIPlayerRelicsComponent>(entity);
		uprc->baseImage.Release();

		for (int i = 0; i < uprc->images.size(); i++)
			uprc->images[i].Release();
		uprc->images.~ML_Vector();

		registry.RemoveComponent<UIPlayerRelicsComponent>(entity);
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	uint16_t destCount = DestroyEntities(entities);
}