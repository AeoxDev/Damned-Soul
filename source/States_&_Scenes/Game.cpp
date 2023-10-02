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
#include "RenderableComponent.h"

#include "Menus.h"
#include "UIRenderer.h"

UINT activeMetadata = 0;

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
		//registry.AddComponent<RenderableComponent>(dog);
		//RenderableComponent* renderCo = registry.GetComponent<RenderableComponent>(dog);
		//renderCo->ToRenderableComponent(renderStates[backBufferRenderSlot]);
		
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
		
		EntityID particleEntity = registry.CreateEntity();
		registry.AddComponent<ParticleComponent>(particleEntity);
		ParticleComponent* particleComp = registry.GetComponent<ParticleComponent>(particleEntity);
		Particles::PrepareSmokeParticles(particleComp, renderStates, 4.0f, 5.0f, 3.0f, DirectX::XMFLOAT3(0.f, 0.f, 0.f));

		Begin2dFrame(ui);
		HUDCanvas->Render(ui);
		End2dFrame(ui);
	}
}

void GameScene::ComputeShaders()
{
	for (auto pEntity : View<ParticleComponent>(registry))
	{
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);
		if (pComp->metadataSlot >= 0)
		{
			activeMetadata++;
		}
	}

	Particles::PrepareParticleCompute(renderStates);
	Dispatch(256, activeMetadata, 0);
	Particles::FinishParticleCompute(renderStates);
}
void GameScene::Render()
{
	ClearBackBuffer();
	RenderUI();
	//Render Geometry
	
	////Set shaders here.
	//PrepareBackBuffer();
	//for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	//{
	//	PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
	//	DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ player->goalX,player->goalZ, 0.0f };
	//	DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
	//	DirectX::XMFLOAT3 l;
	//	DirectX::XMStoreFloat3(&l, length);
	//	float angle = acosf(player->dirX);
	//	if (player->dirZ < 0.0f)
	//	{
	//		angle *= -1.0f;
	//	}
	//	if (l.x > 0.001f)
	//	{
	//		goalV = DirectX::XMVector2Normalize(goalV);

	//		DirectX::XMFLOAT3 goalFloats;
	//		DirectX::XMStoreFloat3(&goalFloats, goalV);
	//		player->goalX = goalFloats.x;
	//		player->goalZ = goalFloats.y;
	//		float goalAngle = acosf(player->goalX);
	//		if (player->goalZ < 0.0f)
	//		{
	//			goalAngle *= -1.0f;
	//		}
	//		//Check if shortest distance is right or left
	//		float orthogonalX = -player->dirZ;
	//		float orthogonalZ = player->dirX;
	//		//float dot = playerInputs[i].aimDirection.Dot(players[i].forward);
	//		float dot = player->goalX * player->dirX + player->goalZ * player->dirZ;
	//		float orthDot = player->goalX * orthogonalX + player->goalZ * orthogonalZ;
	//		if (orthDot > 0.0f)
	//		{//Om till vänster
	//			angle += GetDeltaTime() * (10.1f - dot);
	//		}
	//		else
	//		{
	//			angle -= GetDeltaTime() * (10.1f - dot);
	//		}
	//		player->dirX = cosf(angle);
	//		player->dirZ = sinf(angle);
	//		player->goalX = 0.0f;
	//		player->goalZ = 0.0f;
	//	}

	//	SetWorldMatrix(player->posX, player->posY, player->posZ, -player->dirX, 0.0f, player->dirZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
	//	SetVertexBuffer(player->model->m_vertexBuffer);
	//	SetIndexBuffer(player->model->m_indexBuffer);
	//	//RenderIndexed(player->model->m_bonelessModel->m_numIndices);
	//	player->model->RenderAllSubmeshes();
	//}
	
	//for (auto entity : View<ModelComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	//{
	//	ModelComponent* dogCo = registry.GetComponent<ModelComponent>(entity);
	//	dogCo->model.RenderAllSubmeshes();
	//	RenderIndexed(dogCo->model.m_bonelessModel->m_numIndices);
	//}

	//UnloadVertexBuffer();
	//UnloadIndexBuffer();

	//Set all the shaders
	Particles::PrepareParticlePass(renderStates);
	//Loop to find all metadata that are active,
	//Render
	for (auto pEntity : View<ParticleComponent>(registry))
	{
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);
		if (pComp->metadataSlot >= 0)
		{
			RenderOffset(THREADS_PER_GROUP, pComp->metadataSlot * THREADS_PER_GROUP);
		}
	}
	Particles::FinishParticlePass();
	activeMetadata = 0;
}
void GameScene::Input()
{
	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInMainMenu(true);
		SetInPlay(false);
		stateManager.menu.Setup();
		Clear();
		Unload();
	}
	if (keyInput[SDL_SCANCODE_W] == down)
	{
		for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
		{
			PlayerComponent* p = registry.GetComponent<PlayerComponent>(entity);
			p->posZ += p->movementSpeed * GetDeltaTime();
			p->goalZ += 1.0f;
		}
	}
	if (keyInput[SDL_SCANCODE_S] == down)
	{
		for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
		{
			PlayerComponent* p = registry.GetComponent<PlayerComponent>(entity);
			p->posZ -= p->movementSpeed * GetDeltaTime();
			p->goalZ -= 1.0f;
		}
	}
	if (keyInput[SDL_SCANCODE_A] == down)
	{
		for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
		{
			PlayerComponent* p = registry.GetComponent<PlayerComponent>(entity);
			p->posX -= p->movementSpeed * GetDeltaTime();
			p->goalX -= 1.0f;
		}
	}
	if (keyInput[SDL_SCANCODE_D] == down)
	{
		for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
		{
			PlayerComponent* p = registry.GetComponent<PlayerComponent>(entity);
			p->posX += p->movementSpeed * GetDeltaTime();
			p->goalX += 1.0f;
		}
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