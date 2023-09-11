#pragma once
#include <string>
#include <iostream>

#include "SceneManager.h"
#include "ButtonManager.h"

enum class State {
	Menu,
	Game
};

enum class MenuState {
	Main,
	Settings,
	Credits
};

enum class GameState {
	Unpause,
	Pause,
	Settings
};

enum class SettingsState {
	Graphics,
	Audio,
	Controls,
};

struct Settings
{
	SettingsState currentSubState = SettingsState::Graphics;
	std::pair<int, int> resolution = { sdl.WIDTH,  sdl.HEIGHT };

	void HandleKeyInputs(int keyInput[])
	{
		switch (currentSubState)
		{
		case SettingsState::Graphics:
			std::cout << "Graphics\n";

			if (keyInput[SDL_SCANCODE_LEFT])
				currentSubState = SettingsState::Controls;

			if (keyInput[SDL_SCANCODE_RIGHT])
				currentSubState = SettingsState::Audio;

			break;
		case SettingsState::Audio:
			std::cout << "Audio\n";

			if (keyInput[SDL_SCANCODE_LEFT])
				currentSubState = SettingsState::Graphics;

			if (keyInput[SDL_SCANCODE_RIGHT])
				currentSubState = SettingsState::Controls;

			break;
		case SettingsState::Controls:
			std::cout << "Controls\n";

			if (keyInput[SDL_SCANCODE_LEFT])
				currentSubState = SettingsState::Audio;

			if (keyInput[SDL_SCANCODE_RIGHT])
				currentSubState = SettingsState::Graphics;

			break;
		}
	};

	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, std::pair<int, int> mousePos)
	{
		switch (currentSubState)
		{
		case SettingsState::Graphics:
			std::cout << "Graphics\n";

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Audio").Intersects(mousePos))
				buttonManager.DoButtonAction("Audio");

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Controls").Intersects(mousePos))
				buttonManager.DoButtonAction("Controls");

			break;
		case SettingsState::Audio:
			std::cout << "Audio\n";

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Graphics").Intersects(mousePos))
				buttonManager.DoButtonAction("Graphics");

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Controls").Intersects(mousePos))
				buttonManager.DoButtonAction("Controls");

			break;
		case SettingsState::Controls:
			std::cout << "Controls\n";

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Graphics").Intersects(mousePos))
				buttonManager.DoButtonAction("Graphics");

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Audio").Intersects(mousePos))
				buttonManager.DoButtonAction("Audio");

			break;
		}
	};
};

struct Game
{
	GameState currentSubState = GameState::Unpause;
	SceneManager sceneManager;

	std::pair<int, int> playerPosition = { 200, 200 };

	void Update()
	{
		sceneManager.Update();
	};

	void HandleKeyInputs(int keyInput[], Settings& settings)
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
	};
	
	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
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
	};

	void Reset()
	{
		currentSubState = GameState::Unpause;
		playerPosition = { 200, 200 };
		sceneManager = {};
	};
};

struct Menu
{
	MenuState currentSubState = MenuState::Main;

	void HandleKeyInputs(int keyInput[], Settings& settings)
	{
		switch (currentSubState)
		{
		case MenuState::Main:
			std::cout << "Main\n";

			break;
		case MenuState::Settings:
			std::cout << "Settings\n";

			if (keyInput[SDL_SCANCODE_ESCAPE])
				currentSubState = MenuState::Main;

			settings.HandleKeyInputs(keyInput);
			break;
		case MenuState::Credits:
			std::cout << "Credits\n";

			if (keyInput[SDL_SCANCODE_ESCAPE])
				currentSubState = MenuState::Main;

			break;
		}
	};

	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
	{
		switch (currentSubState)
		{
		case MenuState::Main:
			std::cout << "Main\n";

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Play").Intersects(mousePos))
				buttonManager.DoButtonAction("Play");

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("MenuSettings").Intersects(mousePos))
				buttonManager.DoButtonAction("MenuSettings");

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Credits").Intersects(mousePos))
				buttonManager.DoButtonAction("Credits");

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Exit").Intersects(mousePos))
				buttonManager.DoButtonAction("Exit");

			break;
		case MenuState::Settings:
			std::cout << "Settings\n";

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Main").Intersects(mousePos))
				buttonManager.DoButtonAction("Main");

			settings.HandleMouseInputs(mouseEvent, buttonManager, mousePos);
			break;
		case MenuState::Credits:
			std::cout << "Credits\n";

			if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Main").Intersects(mousePos))
				buttonManager.DoButtonAction("Main");

			break;
		}
	};
};