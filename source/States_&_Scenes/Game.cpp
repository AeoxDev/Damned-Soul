#include "States_&_Scenes\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"

Model dogModel;

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
	Render(dogModel.m_bonelessModel->m_numIndices);
	
	Particles::PrepareParticlePass();
	SetTopology(POINTLIST);
	Render(100);
	Particles::FinishParticlePass();*/

}

void GameScene::Clear()
{
}

void GameScene::Setup(int scene)
{
	if (-1 == dogModel.m_indexBuffer)
		dogModel.Load("HellhoundDummy_PH.mdl");
}

void GameScene::ComputeShaders()
{
	Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();
}
void GameScene::Render()
{
}
void GameScene::Input()
{
}
void GameScene::Unload()
{
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
