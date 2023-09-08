#pragma once
#include "SDLHandler.h"
#include "StateStructures.h"
#include "ButtonManager.h"

class StateManager
{
private:
	State m_currentState = State::Menu;

	Game m_game;
	Menu m_menu;
	Settings m_settings;

	ButtonManager m_buttonManager;
	std::pair<int, int> mousePos;
	std::pair<int, int> oldmousepos = {};

public:
	StateManager();

	void Update();
	void HandleKeyInputs(int[]);
	void HandleMouseInputs(SDL_MouseButtonEvent);

};