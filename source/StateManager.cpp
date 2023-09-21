#include "States_&_Scenes\StateManager.h"

State currentStates;

//void StateManager::ReadKeyInputs(int keyState[])
//{
//	switch (m_currentState)
//	{
//	case State::Menu:
//
//		m_menu.ReadKeyInputs(keyState, m_settings);
//		break;
//	case State::Game:
//
//		m_game.ReadKeyInputs(keyState, m_settings);
//		break;
//	}
//}
//
//void StateManager::ReadKeyOutputs(int keyState[])
//{
//	switch (m_currentState)
//	{
//	case State::Menu:
//
//		m_menu.ReadKeyOutputs(keyState, m_settings);
//		break;
//	case State::Game:
//
//		m_game.ReadKeyOutputs(keyState, m_settings);
//		break;
//	}
//}
//
//void StateManager::ReadMouseInputs(SDL_MouseButtonEvent mouseEvent)
//{
//	switch (m_currentState)
//	{
//	case State::Menu:
//
//		m_menu.ReadMouseInputs(mouseEvent, m_buttonManager, m_settings, mousePos);
//		break;
//	case State::Game:
//
//		m_game.ReadMouseInputs(mouseEvent, m_buttonManager, m_settings, mousePos);
//		break;
//	}
//}
//
//void StateManager::ReadMouseOutputs(SDL_MouseButtonEvent mouseEvent)
//{
//	switch (m_currentState)
//	{
//	case State::Menu:
//
//		m_menu.ReadMouseOutputs(mouseEvent, m_buttonManager, m_settings, mousePos);
//		break;
//	case State::Game:
//
//		m_game.ReadMouseOutputs(mouseEvent, m_buttonManager, m_settings, mousePos);
//		break;
//	}
//}

void StateManager::Setup()
{
	//
}

void StateManager::Clear()
{
	//Find which registries for clear system
}

void StateManager::Render()
{
	//Find which registries for render system
}

void StateManager::Update()
{
	if (currentStates & State::MainMenu)
	{
		this->m_menu.MenuSystem();
	}
	//Find which registries to update
	//switch (m_currentState)
	//{
	//case State::Menu:

	//	if (m_menu.currentSubState == MenuState::Main)
	//		m_menu.Update();
	//	break;
	//case State::Game:

	//	if (m_game.currentSubState == GameState::Unpause)
	//	{
	//		//In the future, we may not want to update particles on every update call
	//		//Currently lacking an indicator as to when we want to update particles
	//		m_game.UpdateParticles();

	//		m_game.Update();
	//	}

	//	break;
	//}

	//SDL_GetMouseState(&mousePos.first, &mousePos.second);
	//if (mousePos != oldmousepos)
	//	//std::cout << mousePos.first << ", " << mousePos.second << std::endl;

	//oldmousepos = mousePos;
}
//
//void StateManager::ReadInputs()
//{
//	while (SDL_PollEvent(&sdl.sdlEvent))
//	{
//
//		if (sdl.sdlEvent.key.repeat == 0)
//		{
//			switch (sdl.sdlEvent.type)
//			{
//			case SDL_QUIT: //User requests quit
//				sdl.quit = true;
//				break;
//			case SDL_KEYDOWN: //Reads that a key has been pressed
//				m_keyInput[sdl.sdlEvent.key.keysym.scancode] = true;
//				ReadKeyInputs(m_keyInput);
//				break;
//			case SDL_KEYUP: //Reads that a key has been released
//				ReadKeyOutputs(m_keyInput);
//				m_keyInput[sdl.sdlEvent.key.keysym.scancode] = false;
//				break;
//			case SDL_MOUSEBUTTONDOWN: //Reads that a mouse button has been pressed
//				ReadMouseInputs(sdl.sdlEvent.button);
//				break;
//			case SDL_MOUSEBUTTONUP: //Reads that a mouse button has been released
//				ReadMouseOutputs(sdl.sdlEvent.button);
//				break;
//			}
//		}
//	}
//}
