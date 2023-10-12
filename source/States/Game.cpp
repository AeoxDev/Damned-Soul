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

		
		Camera::ResetCamera();

		//Entities
		EntityID gameLevel = registry.CreateEntity();
		EntityID dog = registry.CreateEntity();
		EntityID stage = registry.CreateEntity();
		EntityID player = registry.CreateEntity();
		EntityID skeleton = registry.CreateEntity();
		EntityID skeleton2 = registry.CreateEntity();
		EntityID eye = registry.CreateEntity();
		EntityID particle = registry.CreateEntity();

		registry.AddComponent<ModelBonelessComponent>(dog, LoadModel("HellhoundDummy_PH.mdl"));
		registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
		registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
		registry.AddComponent<AnimationComponent>(player, AnimationComponent());
		registry.AddComponent<ModelBonelessComponent>(skeleton, LoadModel("SkeletonOneDymmy.mdl"));
		registry.AddComponent<ModelBonelessComponent>(skeleton2, LoadModel("SkeletonOneDymmy.mdl"));
		registry.AddComponent<ModelBonelessComponent>(eye, LoadModel("FlyingEyeDymmy.mdl"));


		// Dog
		TransformComponent dogTransformComponent;
		dogTransformComponent.facingX = 1.0f; dogTransformComponent.positionX = 20.0f; dogTransformComponent.facingX = 1.0f;
		registry.AddComponent<TransformComponent>(dog, dogTransformComponent);
		
		// Stage (Default)
		registry.AddComponent<TransformComponent>(stage);
		
		// Player (Default)
		registry.AddComponent<TransformComponent>(player);
		
		// First skeleton
		TransformComponent fsTransformComponent;
		fsTransformComponent.positionZ = 20.0f;
		registry.AddComponent<TransformComponent>(skeleton, fsTransformComponent);
		
		// Second skeleton
		TransformComponent ssTransformComponent;
		ssTransformComponent.positionZ = 15.0f;
		registry.AddComponent<TransformComponent>(skeleton2, ssTransformComponent);

		// Eye
		TransformComponent eyeTransformComponent;
		eyeTransformComponent.positionZ = 15.f;
		eyeTransformComponent.positionX = 15.f;
		registry.AddComponent<TransformComponent>(eye, eyeTransformComponent);


		registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		registry.AddComponent<PlayerComponent>(player);

		registry.AddComponent<StatComponent>(dog, 50.f, 20.f, 25.f, 5.f);
		registry.AddComponent<EnemyComponent>(dog, 1);

		registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
		registry.AddComponent<EnemyComponent>(skeleton, 2);

		registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);
		registry.AddComponent<EnemyComponent>(skeleton2, 2);

		registry.AddComponent<StatComponent>(eye, 50.f, 20.f, 25.f, 5.f);
		registry.AddComponent<EnemyComponent>(eye, 1);

		registry.AddComponent<ControllerComponent>(player);

				ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
		PointOfInterestComponent poic;
		poic.weight = 10.0f;
		registry.AddComponent<PointOfInterestComponent>(player, poic);
		//registry.AddComponent<PointOfInterestComponent>(dog);
		//registry.AddComponent<PointOfInterestComponent>(skeleton);
		//registry.AddComponent<PointOfInterestComponent>(skeleton2);
		registry.AddComponent<PointOfInterestComponent>(eye);

		//ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
		////particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

		UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
		UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));

	
		std::string valueAsString = std::to_string(scene);
		std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());
		UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, 1.0f, DirectX::XMFLOAT2(0.9f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/CheckboxBase.png"), UIText(valueAsWString));
		

		//registry.AddComponent<HellhoundBehaviour>(dog);
		//registry.AddComponent<SkeletonBehaviour>(skeleton);
		//registry.AddComponent<SkeletonBehaviour>(skeleton2);
		registry.AddComponent<EyeBehaviour>(eye);


		RenderGeometryIndependentCollision(stage);
		//Finally set the collision boxes
		SetupPlayerCollisionBox(player, 1.0f);

		SetupEnemyCollisionBox(skeleton, 0.9f);
		SetupEnemyCollisionBox(skeleton2, 0.9f);
		SetupEnemyCollisionBox(dog, 1.0f);
		SetupEnemyCollisionBox(eye, 1.0f);

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
		registry.RemoveComponent<ModelBonelessComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<TransformComponent>(registry))
	{
		registry.RemoveComponent<TransformComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		registry.RemoveComponent<ModelSkeletonComponent>(entity);

		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<AnimationComponent>(registry))
	{
		registry.RemoveComponent<AnimationComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<GeometryIndependentComponent>(registry))
	{
		registry.RemoveComponent<GeometryIndependentComponent>(entity);

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

	for (auto entity : View<ControllerComponent>(registry))
	{
		registry.RemoveComponent<ControllerComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<StatComponent>(registry))
	{
		registry.RemoveComponent<StatComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<SkeletonBehaviour>(registry))
	{
		registry.RemoveComponent<SkeletonBehaviour>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<HellhoundBehaviour>(registry))
	{
		registry.RemoveComponent<HellhoundBehaviour>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<HitboxComponent>(registry))
	{
		registry.RemoveComponent<HitboxComponent>(entity);

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

	uint16_t destCount = DestroyEntities(entities);
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload();
	stateManager.menu.Setup();
}
