#pragma once
#include "SDLHandler.h"

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

struct Game
{
	GameState currentSubState = GameState::Unpause;

};

struct Menu
{
	MenuState currentSubState = MenuState::Main;

};

struct Settings
{
	SettingsState currentSubState = SettingsState::Graphics;
	std::pair<int, int> resolution;
};

void HandleMenuStateInput(Menu&, Settings&, int[]);
void HandleGameStateInput(Game&, Settings&, int[]);
void HandleSettingsStateInput(Settings& , int[]);

std::string stateTemp = "";
std::string stateMenuTemp = "";
std::string stateGameTemp = "";
std::string stateSettingsTemp = "";

void HandleStateInput(State& currentMainState, Game& game, Menu& menu, Settings& settings)
{
	while (SDL_PollEvent(&sdl.sdlEvent) != 0)
	{
		int input[256]{0};
		
		std::string Menu = "Menu";
		std::string Game = "Game";

		if (sdl.sdlEvent.type == SDL_KEYDOWN)
			input[sdl.sdlEvent.key.keysym.scancode] = true;

		if (sdl.sdlEvent.type == SDL_KEYUP)
			input[sdl.sdlEvent.key.keysym.scancode] = false;
		
		//User requests quit
		if (sdl.sdlEvent.type == SDL_QUIT)
			sdl.quit = true;

		switch (currentMainState)
		{
		case State::Menu:
			if (stateTemp != Menu)
				std::cout << Menu << std::endl;

			if (input[SDL_SCANCODE_TAB] && menu.currentSubState == MenuState::Main)
			{
				stateGameTemp = "";
				currentMainState = State::Game;
				break;
			}
			HandleMenuStateInput(menu, settings, input);
			stateTemp = Menu;
			break;
		case State::Game:
			if (stateTemp != Game)
				std::cout << Game << std::endl;

			if (input[SDL_SCANCODE_TAB] && game.currentSubState == GameState::Pause)
			{
				stateMenuTemp = "";
				currentMainState = State::Menu;
				game.currentSubState = GameState::Unpause;
				break;
			}
			HandleGameStateInput(game, settings, input);
			stateTemp = Game;
			break;
		default:
			break;
		}
		
	}
}

void HandleMenuStateInput(Menu& menu, Settings& settings, int input[])
{
	std::string Main = "Main";
	std::string Settings = "Menu Settings";
	std::string Credits = "Credits";

	switch (menu.currentSubState)
	{
	case MenuState::Main:
		if (stateMenuTemp != Main)
			std::cout << Main << std::endl << std::endl;

		if (input[SDL_SCANCODE_UP])
			menu.currentSubState = MenuState::Settings;

		if (input[SDL_SCANCODE_DOWN])
			menu.currentSubState = MenuState::Credits;

		stateMenuTemp = Main;
		break;
	case MenuState::Settings:
		if (stateMenuTemp != Settings)
			std::cout << Settings << std::endl << std::endl;

		if (input[SDL_SCANCODE_ESCAPE])
		{
			stateSettingsTemp = "";
			menu.currentSubState = MenuState::Main;
		}

		if (input[SDL_SCANCODE_F])
		{
			std::cout << "Changed Resolution to 1280x800\n";
			settings.resolution = { 1280, 800 };
		}

		HandleSettingsStateInput(settings, input);

		stateMenuTemp = Settings;
		break;
	case MenuState::Credits:
		if (stateMenuTemp != Credits)
			std::cout << Credits << std::endl << std::endl;

		if (input[SDL_SCANCODE_ESCAPE])
			menu.currentSubState = MenuState::Main;

		stateMenuTemp = Credits;
		break;
	default:
		break;
	}

}

void HandleGameStateInput(Game& game, Settings& settings, int input[])
{
	
	std::string Unpause = "Unpause";
	std::string Pause = "Pause";
	std::string Settings = "Game Settings";

	switch (game.currentSubState)
	{
	case GameState::Unpause:
		if (stateGameTemp != Unpause)
			std::cout << Unpause << std::endl << std::endl;

		if (input[SDL_SCANCODE_ESCAPE])
			game.currentSubState = GameState::Pause;

		stateGameTemp = Unpause;
		break;
	case GameState::Pause:
		if (stateGameTemp != Pause)
			std::cout << Pause << std::endl << std::endl;

		if (input[SDL_SCANCODE_UP])
			game.currentSubState = GameState::Unpause;


		if (input[SDL_SCANCODE_DOWN])
			game.currentSubState = GameState::Settings;

		stateGameTemp = Pause;
		break;
	case GameState::Settings:
		if (stateGameTemp != Settings)
			std::cout << Settings << std::endl << std::endl;

		if (input[SDL_SCANCODE_ESCAPE])
		{
			stateGameTemp = "";
			game.currentSubState = GameState::Pause;
		}

		if (input[SDL_SCANCODE_F])
		{
			std::cout << "Changed Resolution to 1600x900\n";
			settings.resolution = {1600, 900};
		}

		HandleSettingsStateInput(settings, input);

		stateGameTemp = Settings;
		break;
	default:
		break;
	}

}

inline void HandleSettingsStateInput(Settings& settings, int input[])
{
	std::string Graphics = "Graphics";
	std::string Audio = "Audio";
	std::string Controls = "Controls";

	switch (settings.currentSubState)
	{
	case SettingsState::Graphics:
		if (stateSettingsTemp != Graphics)
			std::cout << Graphics << std::endl << "Resolution: (" << settings.resolution.first << "x" << settings.resolution.second << ")\n" << std::endl;

		if (input[SDL_SCANCODE_LEFT])
			settings.currentSubState = SettingsState::Controls;

		if (input[SDL_SCANCODE_RIGHT])
			settings.currentSubState = SettingsState::Audio;

		stateSettingsTemp = Graphics;
		break;
	case SettingsState::Audio:
		if (stateSettingsTemp != Audio)
			std::cout << Audio << std::endl << std::endl;

		if (input[SDL_SCANCODE_LEFT])
			settings.currentSubState = SettingsState::Graphics;

		if (input[SDL_SCANCODE_RIGHT])
			settings.currentSubState = SettingsState::Controls;

		stateSettingsTemp = Audio;
		break;
	case SettingsState::Controls:
		if (stateSettingsTemp != Controls)
			std::cout << Controls << std::endl << std::endl;

		if (input[SDL_SCANCODE_LEFT])
			settings.currentSubState = SettingsState::Audio;

		if (input[SDL_SCANCODE_RIGHT])
			settings.currentSubState = SettingsState::Graphics;

		stateSettingsTemp = Controls;
		break;
	default:
		break;
	}
}

