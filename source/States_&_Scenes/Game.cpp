#include "States_&_Scenes\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"

Model dogModel;

void Game::Update()
{
	if (-1 == dogModel.m_indexBuffer)
		dogModel.Load("Hellhound.mdl");

	// No support for held keys in the input handler at the moment
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	float move[2] = { 0, 0 };

	// Move left
	if (keystate[SDL_SCANCODE_A])
		move[0] -= 1;
	// Move right
	if (keystate[SDL_SCANCODE_D])
		move[0] += 1;
	// Move up
	if (keystate[SDL_SCANCODE_W])
		move[1] += 1;
	// Move down
	if (keystate[SDL_SCANCODE_S])
		move[1] -= 1;

	float len = sqrt(move[0] * move[0] + move[1] * move[1]);
	float scale = GetAverage() / (len < 0.1f ? 1 : len);
	playerPosition[0] += move[0] * scale;
	playerPosition[1] += move[1] * scale;

	Camera::SetPosition(playerPosition[0], playerPosition[1], -2.0f);
	Camera::SetLookAt(playerPosition[0], playerPosition[1], 0.0f);
	Camera::UpdateView();
	dogModel.SetVertexAndIndexBuffersActive();
	Render(dogModel.m_bonelessModel->m_numIndices);
	
	sceneManager.Update();
}

void Game::HandleKeyInputs(int keyInput[], Settings& settings)
{
	switch (currentSubState)
	{
	case GameState::Unpause:
		std::cout << "Unpause\n";

		if (keyInput[SDL_SCANCODE_0])
			sceneManager.SetScene("Shop");

		else if (keyInput[SDL_SCANCODE_1] && sceneManager.GetCurrentSceneName() == "Shop")
		{
			std::string name = "Level_1";
			sceneManager.SetScene(name);
		}

		else if (keyInput[SDL_SCANCODE_2] && sceneManager.GetCurrentSceneName() == "Level_1")
		{
			std::vector<std::string> entityList = { "Imp", "Imp" , "Skeleton" };
			std::string name = "Level_2";
			sceneManager.SetScene(name);
			sceneManager.AddScene(name, entityList);
		}

		else if (keyInput[SDL_SCANCODE_3] && sceneManager.GetCurrentSceneName() == "Level_2")
		{
			std::vector<std::string> entityList = { "Skeleton", "Skeleton" , "Demon" };
			std::string name = "Level_3";
			sceneManager.SetScene(name);
			sceneManager.AddScene(name, entityList);
		}

		else if (keyInput[SDL_SCANCODE_Q])
			sceneManager.WriteEntities();

		else if (keyInput[SDL_SCANCODE_ESCAPE])
			currentSubState = GameState::Pause;

		break;
	case GameState::Pause:
		std::cout << "Pause\n";

		break;
	case GameState::Settings:
		std::cout << "Settings\n";

		if (keyInput[SDL_SCANCODE_ESCAPE])
			currentSubState = GameState::Pause;

		settings.HandleKeyInputs(keyInput);
		break;
	}
}

void Game::HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
{
	switch (currentSubState)
	{
	case GameState::Unpause:
		std::cout << "Unpause\n";

		break;
	case GameState::Pause:
		std::cout << "Pause\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("MainMenu").Intersects(mousePos))
			buttonManager.DoButtonAction("MainMenu");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("GameSettings").Intersects(mousePos))
			buttonManager.DoButtonAction("GameSettings");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Resume").Intersects(mousePos))
			buttonManager.DoButtonAction("Resume");

		break;
	case GameState::Settings:
		std::cout << "Settings\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Pause").Intersects(mousePos))
			buttonManager.DoButtonAction("Pause");

		settings.HandleMouseInputs(mouseEvent, buttonManager, mousePos);
		break;
	}
}

void Game::Reset()
{
	currentSubState = GameState::Unpause;
	playerPosition[0] = playerPosition[1] = playerPosition[2] = 0;
	sceneManager = {};
}
