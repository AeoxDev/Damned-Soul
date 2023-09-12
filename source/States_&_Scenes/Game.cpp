#include "States_&_Scenes\Game.h"

void Game::Update()
{
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

		if (keyInput[SDL_SCANCODE_1] && sceneManager.GetCurrentSceneName() == "Shop")
		{
			std::string name = "Level_1";
			sceneManager.SetScene(name);
		}

		if (keyInput[SDL_SCANCODE_2] && sceneManager.GetCurrentSceneName() == "Level_1")
		{
			std::vector<std::string> entityList = { "Imp", "Imp" , "Skeleton" };
			std::string name = "Level_2";
			sceneManager.SetScene(name);
			sceneManager.AddScene(name, entityList);
		}

		if (keyInput[SDL_SCANCODE_3] && sceneManager.GetCurrentSceneName() == "Level_2")
		{
			std::vector<std::string> entityList = { "Skeleton", "Skeleton" , "Demon" };
			std::string name = "Level_3";
			sceneManager.SetScene(name);
			sceneManager.AddScene(name, entityList);
		}

		if (keyInput[SDL_SCANCODE_Q])
			sceneManager.WriteEntities();

		if (keyInput[SDL_SCANCODE_ESCAPE])
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
	playerPosition = { 200, 200 };
	sceneManager = {};
}
