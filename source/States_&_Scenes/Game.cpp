#include "States_&_Scenes\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"
#include "Components.h"
#include "Input.h"
#include "States_&_Scenes\StateManager.h"
#include "Registry.h"
#include "Menus.h"
#include "UIRenderer.h"

void GameScene::Update()
{
	for (auto entity : View<PlayerComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ player->goalX,player->goalZ, 0.0f };
		DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
		DirectX::XMFLOAT3 l;
		DirectX::XMStoreFloat3(&l, length);
		float angle = acosf(player->dirX);
		if (player->dirZ < 0.0f)
		{
			angle *= -1.0f;
		}
		if (l.x > 0.001f)
		{
			goalV = DirectX::XMVector2Normalize(goalV);

			DirectX::XMFLOAT3 goalFloats;
			DirectX::XMStoreFloat3(&goalFloats, goalV);
			player->goalX = goalFloats.x;
			player->goalZ = goalFloats.y;
			float goalAngle = acosf(player->goalX);
			if (player->goalZ < 0.0f)
			{
				goalAngle *= -1.0f;
			}
			//Check if shortest distance is right or left
			float orthogonalX = -player->dirZ;
			float orthogonalZ = player->dirX;
			//float dot = playerInputs[i].aimDirection.Dot(players[i].forward);
			float dot = player->goalX * player->dirX + player->goalZ * player->dirZ;
			float orthDot = player->goalX * orthogonalX + player->goalZ * orthogonalZ;
			if (orthDot > 0.0f)
			{//Om till vänster
				angle += GetDeltaTime() * (10.1f - dot);
			}
			else
			{
				angle -= GetDeltaTime() * (10.1f - dot);
			}
			player->dirX = cosf(angle);
			player->dirZ = sinf(angle);
			player->goalX = 0.0f;
			player->goalZ = 0.0f;
		}

		SetWorldMatrix(player->posX, player->posY, player->posZ, -player->dirX, 0.0f, player->dirZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
	}
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

	/*	registry.AddComponent<UICanvas>(GameHUD);
		UICanvas* HUDCanvas = registry.GetComponent<UICanvas>(GameHUD);
		SetupHUDCanvas(*HUDCanvas);
		UpdateUI(*HUDCanvas);*/

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
		ModelComponent* stageCo = registry.AddComponent<ModelComponent>(stage);
		stageCo->model.Load("PlaceholderScene.mdl");

		RenderGeometryIndependentCollision(stage);

		EntityID player = registry.CreateEntity();
		registry.AddComponent<PlayerComponent>(player);
		PlayerComponent* pc = registry.GetComponent<PlayerComponent>(player);
		ModelComponent* playerModel = registry.AddComponent<ModelComponent>(player);
		playerModel->model.Load("HellhoundDummy_PH.mdl");
		/*Begin2dFrame(ui);
		HUDCanvas->Render(ui);
		End2dFrame(ui);*/
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
	

	//for (auto entity : View<ModelComponent>(registry))
	//{
	//	ModelComponent* toRender = registry.GetComponent<ModelComponent>(entity);
	//	SetWorldMatrix(0.0f, 0.0f, 0.0f, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
	//	SetVertexBuffer(toRender->model.m_vertexBuffer);
	//	SetIndexBuffer(toRender->model.m_indexBuffer);
	//	//RenderIndexed(player->model->m_bonelessModel->m_numIndices);
	//	toRender->model.RenderAllSubmeshes();
	//}
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
	//for (auto entity : View<UICanvas>(registry))
	//{
	//	//Get entity with UI, release components.
	//	UICanvas* ui = registry.GetComponent<UICanvas>(entity);
	//	if (ui && ui->header == UI_CANVAS_HEADER)
	//	{
	//		ui->Release();
	//	}
	//}
}