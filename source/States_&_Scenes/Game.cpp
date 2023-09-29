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

	//	SetWorldMatrix(player->posX, player->posY, player->posZ, -player->dirX, 0.0f, player->dirZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
	//}
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
		SetupButtons();
		SetupImages();
		SetupText();
		DrawUi();

		//Doggo
		EntityID dog = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(dog);
		ModelComponent* dogCo = registry.GetComponent<ModelComponent>(dog);
		TransformComponent* dtc = registry.AddComponent<TransformComponent>(dog);
		dtc->facingX = 1.0f;
		dogCo->model.Load("HellhoundDummy_PH.mdl");
		/*EntityID stage = registry.CreateEntity();
		registry.AddComponent<ModelComponent>(stage);
		ModelComponent* stageCo = registry.GetComponent<ModelComponent>(stage);
		stageCo->model.Load("PlaceholderScene.mdl");*/
		EntityID stage = registry.CreateEntity();
		ModelComponent* stageCo = registry.AddComponent<ModelComponent>(stage);
		TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
		stageCo->model.Load("PlaceholderScene.mdl");

		RenderGeometryIndependentCollision(stage);

		EntityID player = registry.CreateEntity();
		registry.AddComponent<PlayerComponent>(player);
		PlayerComponent* pc = registry.GetComponent<PlayerComponent>(player);
		ModelComponent* pmc = registry.AddComponent<ModelComponent>(player);
		pmc->model.Load("HellhoundDummy_PH.mdl");
		TransformComponent* ptc = registry.AddComponent<TransformComponent>(player);

	}
}

void GameScene::SetupButtons()
{

}

void GameScene::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/EmptyHealth.png", { 150.0f, 50.0f }, { 1.0f, 1.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/FullHealth.png", { 150.0f, 50.0f }, { 0.7f, 1.0f }));
}

void GameScene::SetupText()
{

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Current Souls: 0", { 125.0f, 100.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"This is the HUD!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Press ECS to return to main menu!", { sdl.WIDTH / 2.0f - 50.0f, 350.0f }));
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

	for (auto entity : View<ButtonComponent>(registry))
	{
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		registry.DestroyEntity(entity);
	}
}

void GameScene::DrawUi()
{
	ID2D1RenderTarget* rt = ui.GetRenderTarget();

	UpdateUI2();

	Begin2dFrame(ui);

	for (auto entity : View<ButtonComponent>(registry))
		registry.GetComponent<ButtonComponent>(entity)->button.Draw(ui, rt);

	for (auto entity : View<ImageComponent>(registry))

		registry.GetComponent<ImageComponent>(entity)->image.Draw(rt);

	for (auto entity : View<TextComponent>(registry))
		registry.GetComponent<TextComponent>(entity)->text.Draw(ui);

	End2dFrame(ui);
}