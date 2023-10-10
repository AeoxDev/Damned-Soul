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
	m_active = true;
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

	uint16_t destCount = DestroyEntities(entities);
}