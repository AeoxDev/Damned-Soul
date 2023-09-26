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

void GameScene::ComputeShaders()
{
	/*Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();*/
}
void GameScene::Render()
{
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
		SetInPause(true);
		SetInPlay(false);
		stateManager.pause.Setup();
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
}