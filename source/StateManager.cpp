#include "StateManager.h"

void StateManager::Update()
{
	if (m_currentState == State::Game && m_game.currentSubState == GameState::Unpause)
		m_game.Update();
}

void StateManager::HandleInputs(int keyInput[])
{
	switch (m_currentState)
	{
	case State::Menu:

		//Check if the key is TAB and if current menu sub-state is Main, if true enter the Game State
		if (keyInput[SDL_SCANCODE_TAB] && m_menu.currentSubState == MenuState::Main)
		{
			m_currentState = State::Game;
			break;
		}

		m_menu.HandleInputs(keyInput, m_settings);
		break;
	case State::Game:

		//Check if the key is TAB and if current game sub-state is Pause, if true enter the Menu State
		if (keyInput[SDL_SCANCODE_TAB] && m_game.currentSubState == GameState::Pause)
		{
			m_currentState = State::Menu;
			m_game.Reset();
			break;
		}

		m_game.HandleInputs(keyInput, m_settings);
		break;
	}
}