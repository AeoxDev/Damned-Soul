#pragma once
#include <string>
#include <iostream>

#include "SceneManager.h"

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

	void HandleInputs(int keyInput[])
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

	void HandleInputs(int keyInput[], Settings& settings)
	{
		switch (currentSubState)
		{
		case GameState::Unpause:
			
			std::cout << "Unpause\n";

			if (keyInput[SDL_SCANCODE_W])
				playerPosition.second += 10;

			if (keyInput[SDL_SCANCODE_A])
				playerPosition.first -= 10;

			if (keyInput[SDL_SCANCODE_S])
				playerPosition.second -= 10;

			if (keyInput[SDL_SCANCODE_D])
				playerPosition.first += 10;

			if (keyInput[SDL_SCANCODE_1])
				sceneManager.SetScene("Level_1");

			if (keyInput[SDL_SCANCODE_2])
				sceneManager.SetScene("Level_2");

			std::cout << "Player Position: " << playerPosition.first << ", " << playerPosition.second << std::endl;

			if (keyInput[SDL_SCANCODE_ESCAPE])
				currentSubState = GameState::Pause;

			break;
		case GameState::Pause:
			std::cout << "Pause\n";

			if (keyInput[SDL_SCANCODE_UP])
				currentSubState = GameState::Unpause;

			if (keyInput[SDL_SCANCODE_DOWN])
				currentSubState = GameState::Settings;

			break;
		case GameState::Settings:
			std::cout << "Settings\n";

			settings.HandleInputs(keyInput);

			if (keyInput[SDL_SCANCODE_ESCAPE])
				currentSubState = GameState::Pause;

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
	void HandleInputs(int keyInput[], Settings& settings)
	{
		switch (currentSubState)
		{
		case MenuState::Main:
			std::cout << "Main\n";

			if (keyInput[SDL_SCANCODE_UP])
				currentSubState = MenuState::Settings;

			if (keyInput[SDL_SCANCODE_DOWN])
				currentSubState = MenuState::Credits;

			break;
		case MenuState::Settings:
			std::cout << "Settings\n";

			if (keyInput[SDL_SCANCODE_ESCAPE])
				currentSubState = MenuState::Main;

			settings.HandleInputs(keyInput);
			break;
		case MenuState::Credits:
			std::cout << "Credits\n";

			if (keyInput[SDL_SCANCODE_ESCAPE])
				currentSubState = MenuState::Main;

			break;
		}
	};
};