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

	std::pair<int, int> playerPosition = {200, 200};

	void Reset() 
	{
		currentSubState = GameState::Unpause;
		playerPosition = {200, 200};
	};
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

//Functions to handle individual states inputs
void HandleMenuStateInput(Menu&, Settings&, int[]);
void HandleGameStateInput(Game&, Settings&, int[]);
void HandleSettingsStateInput(Settings&, std::string, int[]);

void ReadKeyEvent(int input[])
{
	//Reads that a key has been pressed
	if (sdl.sdlEvent.type == SDL_KEYDOWN)
		input[sdl.sdlEvent.key.keysym.scancode] = true;

	//Reads that a key has been released
	if (sdl.sdlEvent.type == SDL_KEYUP)
		input[sdl.sdlEvent.key.keysym.scancode] = false;
}

//Debug console writing variables
std::string stateMenuTemp = "";
std::string stateGameTemp = "";
std::string stateSettingsTemp = "";
std::pair<int, int> tempPos = {};

void HandleStateInput(State& currentMainState, Game& game, Menu& menu, Settings& settings)
{
	while (SDL_PollEvent(&sdl.sdlEvent) != 0)
	{
		int input[256]{0};

		ReadKeyEvent(input);
		
		//User requests quit
		if (sdl.sdlEvent.type == SDL_QUIT)
			sdl.quit = true;

		switch (currentMainState)
		{
		case State::Menu:
			SDL_SetRenderDrawColor(sdl.sdlRenderer, 200, 0, 0, 255);

			//Check if the key is TAB and if current menu sub-state is Main, if true enter the Game State
			if (input[SDL_SCANCODE_TAB] && menu.currentSubState == MenuState::Main)
			{
				stateGameTemp = "";
				currentMainState = State::Game;
				break;
			}

			HandleMenuStateInput(menu, settings, input);
			break;
		case State::Game:
			SDL_SetRenderDrawColor(sdl.sdlRenderer, 0, 200, 0, 255);

			//Check if the key is TAB and if current game sub-state is Pause, if true enter the Menu State
			if (input[SDL_SCANCODE_TAB] && game.currentSubState == GameState::Pause)
			{
				stateMenuTemp = "";
				currentMainState = State::Menu;
				game.Reset();
				break;
			}

			HandleGameStateInput(game, settings, input);
			break;
		default:
			break;
		}
		
	}
}

void HandleMenuStateInput(Menu& menu, Settings& settings, int input[])
{
	std::string Main = "Menu: Main";
	std::string Credits = "Menu: Credits";

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
		SDL_SetRenderDrawColor(sdl.sdlRenderer, 100, 0, 200, 255);

		if (input[SDL_SCANCODE_ESCAPE])
		{
			stateSettingsTemp = "";
			menu.currentSubState = MenuState::Main;
		}

		HandleSettingsStateInput(settings, "Menu", input);
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
	
	std::string Unpause = "Game: Unpause";
	std::string Pause = "Game: Pause";

	switch (game.currentSubState)
	{
	case GameState::Unpause:
		if (stateGameTemp != Unpause)
			std::cout << Unpause << std::endl << std::endl;

		//SDL_GetMouseState(&game.playerPosition.first, &game.playerPosition.second);

		/*if (input[SDL_SCANCODE_W])
			game.playerPosition.second += 10;

		if (input[SDL_SCANCODE_A])
			game.playerPosition.first -= 10;

		if (input[SDL_SCANCODE_S])
			game.playerPosition.second -= 10;

		if (input[SDL_SCANCODE_D])
			game.playerPosition.first += 10;

		if (tempPos != game.playerPosition)
			std::cout << "Player Position: " << game.playerPosition.first << ", " << game.playerPosition.second << std::endl;*/

		if (input[SDL_SCANCODE_ESCAPE])
			game.currentSubState = GameState::Pause;

		tempPos = game.playerPosition;
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
		SDL_SetRenderDrawColor(sdl.sdlRenderer, 0, 100, 200, 255);

		if (input[SDL_SCANCODE_ESCAPE])
		{
			stateGameTemp = "";
			game.currentSubState = GameState::Pause;
		}

		HandleSettingsStateInput(settings, "Game", input);
		break;
	default:
		break;
	}

}

inline void HandleSettingsStateInput(Settings& settings, std::string origin, int input[])
{
	std::string Graphics = origin + ": Graphics";
	std::string Audio = origin + ": Audio";
	std::string Controls = origin + ": Controls";

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

