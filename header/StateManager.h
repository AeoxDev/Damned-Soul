#pragma once
#include "SDLHandler.h"
#include "StateStructures.h"

class StateManager
{
private:
	State m_currentState = State::Menu;

	Game m_game;
	Menu m_menu;
	Settings m_settings;

public:

	void HandleInputs(int[]);

};