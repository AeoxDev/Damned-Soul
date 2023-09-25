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

void GameScene::Update()
{
	//float move[2] = { 0, 0 };

	//if (playerDirX != 0)
	//	move[0] += playerDirX;

	//if (playerDirY != 0)
	//	move[1] += playerDirY;

	//float len = sqrt(move[0] * move[0] + move[1] * move[1]);
	//float scale = GetAverage() / (len < 0.1f ? 1 : len);
	//playerPosition[0] += move[0] * scale;
	//playerPosition[1] += move[1] * scale;

	//std::cout << playerPosition[0] << ", " << playerPosition[1] << std::endl;

	/*Camera::SetPosition(playerPosition[0], playerPosition[1], 8.0f);
	Camera::SetLookAt(playerPosition[0], playerPosition[1], 0.0f);
	Camera::UpdateView();

	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	UpdateWorldMatrix(&identity, BIND_VERTEX);

	dogModel.SetMaterialActive();
	dogModel.SetVertexAndIndexBuffersActive();
	SetTopology(TRIANGLELIST);

	dogModel.RenderAllSubmeshes();
	//Render(dogModel.m_bonelessModel->m_numIndices);
	
	Particles::PrepareParticlePass();
	SetTopology(POINTLIST);
	Render(100);
	Particles::FinishParticlePass();*/

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
		registry.AddComponent<RenderableComponent>(dog);
		RenderableComponent* renderCo = registry.GetComponent<RenderableComponent>(dog);

		renderCo->ToRenderableComponent(renderStates[backBufferRenderSlot]);
		
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
//
////void Game::HandleKeyInputs(int keyInput[], Settings& settings)
//void GameScene::ReadKeyInputs(int keyState[], Settings& settings)
//{
//	switch (currentSubState)
//	{
//	case GameState::Unpause:
//		std::cout << "Unpause\n";
//
//		if (keyState[SDL_SCANCODE_W])
//			playerDirY = -1;
//
//		if (keyState[SDL_SCANCODE_A])
//			playerDirX = -1;
//
//		if (keyState[SDL_SCANCODE_S])
//			playerDirY = 1;
//			
//		if (keyState[SDL_SCANCODE_D])
//			playerDirX = 1;
//
//		if (keyState[SDL_SCANCODE_0])
//			sceneManager.SetScene("Shop");
//
//		else if (keyState[SDL_SCANCODE_1] && sceneManager.GetCurrentSceneName() == "Shop")
//		{
//			std::string name = "Level_1";
//			sceneManager.SetScene(name);
//		}
//
//		else if (keyState[SDL_SCANCODE_2] && sceneManager.GetCurrentSceneName() == "Level_1")
//		{
//			std::vector<std::string> entityList = { "Imp", "Imp" , "Skeleton" };
//			std::string name = "Level_2";
//			sceneManager.SetScene(name);
//			sceneManager.AddScene(name, entityList);
//		}
//
//		else if (keyState[SDL_SCANCODE_3] && sceneManager.GetCurrentSceneName() == "Level_2")
//		{
//			std::vector<std::string> entityList = { "Skeleton", "Skeleton" , "Demon" };
//			std::string name = "Level_3";
//			sceneManager.SetScene(name);
//			sceneManager.AddScene(name, entityList);
//		}
//
//		else if (keyState[SDL_SCANCODE_Q])
//			sceneManager.WriteEntities();
//
//		else if (keyState[SDL_SCANCODE_ESCAPE])
//			currentSubState = GameState::Pause;
//
//		break;
//	case GameState::Pause:
//		std::cout << "Pause\n";
//
//		break;
//	case GameState::Settings:
//		std::cout << "Settings\n";
//
//		if (keyState[SDL_SCANCODE_ESCAPE])
//			currentSubState = GameState::Pause;
//
//		settings.ReadKeyInputs(keyState);
//		break;
//	}
//}
//
//void GameScene::ReadKeyOutputs(int keyState[], Settings& settings)
//{
//	switch (currentSubState)
//	{
//	case GameState::Unpause:
//		std::cout << "Unpause\n";
//
//		if (keyState[SDL_SCANCODE_W])
//			playerDirY = 0;
//
//		if (keyState[SDL_SCANCODE_A])
//			playerDirX = 0;
//
//		if (keyState[SDL_SCANCODE_S])
//			playerDirY = 0;
//
//		if (keyState[SDL_SCANCODE_D])
//			playerDirX = 0;
//
//		break;
//	case GameState::Pause:
//		std::cout << "Pause\n";
//
//		break;
//	case GameState::Settings:
//		std::cout << "Settings\n";
//
//		break;
//	}
//}
//
//void GameScene::ReadMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
//{
//	switch (currentSubState)
//	{
//	case GameState::Unpause:
//		std::cout << "Unpause\n";
//
//		break;
//	case GameState::Pause:
//		std::cout << "Pause\n";
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("MainMenu").Intersects(mousePos))
//			buttonManager.DoButtonAction("MainMenu");
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("GameSettings").Intersects(mousePos))
//			buttonManager.DoButtonAction("GameSettings");
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Resume").Intersects(mousePos))
//			buttonManager.DoButtonAction("Resume");
//
//		break;
//	case GameState::Settings:
//		std::cout << "Settings\n";
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Pause").Intersects(mousePos))
//			buttonManager.DoButtonAction("Pause");
//
//		settings.ReadMouseInputs(mouseEvent, buttonManager, mousePos);
//		break;
//	}
//}
//
//void GameScene::ReadMouseOutputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
//{
//	switch (currentSubState)
//	{
//	case GameState::Unpause:
//		std::cout << "Unpause\n";
//
//		break;
//	case GameState::Pause:
//		std::cout << "Pause\n";
//
//		break;
//	case GameState::Settings:
//		std::cout << "Settings\n";
//
//		break;
//	}
//}
//
//void GameScene::Reset()
//{
//	currentSubState = GameState::Unpause;
//	playerPosition[0] = playerPosition[1] = playerPosition[2] = 0;
//	sceneManager = {};
//}
