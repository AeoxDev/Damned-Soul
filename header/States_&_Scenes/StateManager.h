#pragma once
#include "SDLHandler.h"
#include "StateEnums.h"
#include "ButtonManager.h"
#include "States_&_Scenes\Menu.h"
#include "States_&_Scenes\Game.h"

class StateManager
{
private:
	State m_currentState = State::Menu;

	Game m_game;
	Menu m_menu;
	Settings m_settings;

	ButtonManager m_buttonManager;

	int m_keyInput[256]{ 0 };

	//Debug console variables
	std::pair<int, int> mousePos;
	std::pair<int, int> oldmousepos = {};

	void ReadKeyInputs(int[]);
	void ReadKeyOutputs(int[]);

	void ReadMouseInputs(SDL_MouseButtonEvent);
	void ReadMouseOutputs(SDL_MouseButtonEvent);

public:
	StateManager();

	void Update();
	void ReadInputs();

};