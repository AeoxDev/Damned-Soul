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
	Shop,
	Settings
};

enum class SettingsState {
	Graphics,
	Sound,
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

};

void HandleMenuStateInput(Menu& menu, int input[]);
void HandleGameStateInput(Game& game, int input[]);

std::string stateTemp = "";
std::string stateMenuTemp = "";
std::string stateGameTemp = "";

void HandleStateInput(State& currentMainState, Game& game, Menu& menu, Settings& settings)
{
	while (SDL_PollEvent(&sdl.sdlEvent) != 0)
	{
		int input[256]{0};
		
		std::string Menu = "Menu";
		std::string Game = "Game";
		
		//User requests quit
		if (sdl.sdlEvent.type == SDL_QUIT)
			sdl.quit = true;

		if (sdl.sdlEvent.type == SDL_KEYDOWN)
			input[sdl.sdlEvent.key.keysym.scancode] = true;

		if (sdl.sdlEvent.type == SDL_KEYUP)
			input[sdl.sdlEvent.key.keysym.scancode] = false;

		switch (currentMainState)
		{
		case State::Menu:
			if (stateTemp != Menu)
				std::cout << Menu << std::endl;

			if (input[SDL_SCANCODE_TAB] && menu.currentSubState == MenuState::Main)
			{
				stateGameTemp = "";
				currentMainState = State::Game;
			}
			HandleMenuStateInput(menu, input);
			stateTemp = Menu;
			break;
		case State::Game:
			if (stateTemp != Game)
				std::cout << Game << std::endl;

			if (input[SDL_SCANCODE_TAB] && game.currentSubState == GameState::Settings)
			{
				stateMenuTemp = "";
				currentMainState = State::Menu;
			}
			HandleGameStateInput(game, input);
			stateTemp = Game;
			break;
		default:
			break;
		}
		
	}
}

void HandleMenuStateInput(Menu& menu, int input[])
{

	std::string Main = "Main";
	std::string Settings = "Menu Settings";
	std::string Credits = "Credits";
	

	switch (menu.currentSubState)
	{
	case MenuState::Main:
		if (stateMenuTemp != Main)
			std::cout << Main << std::endl << std::endl;

		if (input[SDL_SCANCODE_Q])
			menu.currentSubState = MenuState::Settings;

		stateMenuTemp = Main;
		break;
	case MenuState::Settings:
		if (stateMenuTemp != Settings)
			std::cout << Settings << std::endl << std::endl;

		if (input[SDL_SCANCODE_Q])
			menu.currentSubState = MenuState::Credits;

		stateMenuTemp = Settings;
		break;
	case MenuState::Credits:
		if (stateMenuTemp != Credits)
			std::cout << Credits << std::endl << std::endl;

		if (input[SDL_SCANCODE_Q])
			menu.currentSubState = MenuState::Main;

		stateMenuTemp = Credits;
		break;
	default:
		break;
	}

}

void HandleGameStateInput(Game& game, int input[])
{
	
	std::string Unpause = "Unpause";
	std::string Pause = "Pause";
	std::string Settings = "Game Settings";
	std::string Shop = "Shop";

	switch (game.currentSubState)
	{
	case GameState::Unpause:
		if (stateGameTemp != Unpause)
			std::cout << Unpause << std::endl << std::endl;

		if (input[SDL_SCANCODE_Q])
			game.currentSubState = GameState::Pause;

		stateGameTemp = Unpause;
		break;
	case GameState::Pause:
		if (stateGameTemp != Pause)
			std::cout << Pause << std::endl << std::endl;

		if (input[SDL_SCANCODE_Q])
			game.currentSubState = GameState::Settings;

		if (input[SDL_SCANCODE_E])
			game.currentSubState = GameState::Unpause;

		stateGameTemp = Pause;
		break;
	case GameState::Settings:
		if (stateGameTemp != Settings)
			std::cout << Settings << std::endl << std::endl;

		if (input[SDL_SCANCODE_Q])
			game.currentSubState = GameState::Pause;

		stateGameTemp = Settings;
		break;
	case GameState::Shop:
		if (stateGameTemp != Shop)
			std::cout << Shop << std::endl << std::endl;

		stateGameTemp = Shop;
		break;
	default:
		break;
	}

}