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
	ClearBackBuffer();
}

void GameScene::Setup(int scene)//Load
{
	
	if (scene == 0)
	{
		
		//Setup Game HUD
		EntityID GameHUD = registry.CreateEntity();

		this->registry.AddComponent<UICanvas>(GameHUD);
		UICanvas* HUDCanvas = registry.GetComponent<UICanvas>(GameHUD);
		SetupHUDCanvas(*HUDCanvas);
		UpdateUI(*HUDCanvas);

		//Doggo
		EntityID dog = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(dog);
		ModelComponent* dogCo = registry.GetComponent<ModelComponent>(dog);
		dogCo->model.Load("HellhoundDummy_PH.mdl");
		/*EntityID stage = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(stage);
		ModelComponent* stageCo = registry.GetComponent<ModelComponent>(stage);
		stageCo->model.Load("PlaceholderScene.mdl");*/

		EntityID stage = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(stage);
		ModelComponent* stageCo = registry.GetComponent<ModelComponent>(stage);
		stageCo->model.Load("PlaceholderScene.mdl");

		EntityID giStage = CreateAndRenderGeometryIndependentCollision(registry, stage);
		EntityID player = registry.CreateEntity();
		registry.AddComponent<PlayerComponent>(player);
		PlayerComponent* pc = registry.GetComponent<PlayerComponent>(player);
		pc->model = &dogCo->model;
		
		Begin2dFrame(ui);
		HUDCanvas->Render(ui);
		End2dFrame(ui);
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
	ClearBackBuffer();
	RenderUI();
	//Render Geometry
	
	//Set shaders here.
	PrepareBackBuffer();
	for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		//dogCo->model.RenderAllSubmeshes();
		UpdateWorldMatrix(player->posX, player->posY, player->posZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
		SetVertexBuffer(player->model->m_vertexBuffer);
		SetIndexBuffer(player->model->m_indexBuffer);
		//RenderIndexed(player->model->m_bonelessModel->m_numIndices);
		player->model->RenderAllSubmeshes();
	}
}
void GameScene::Input()
{
	if (keyInput[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInMainMenu(true);
		SetInPlay(false);
		stateManager.menu.Setup();
		Clear();
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
	for (auto entity : View<UICanvas>(registry))
	{
		//Get entity with UI, release components.
		UICanvas* ui = registry.GetComponent<UICanvas>(entity);
		if (ui && ui->header == UI_CANVAS_HEADER)
		{
			ui->Release();
		}
	}
}