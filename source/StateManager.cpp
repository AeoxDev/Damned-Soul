#include "States_&_Scenes\StateManager.h"

void StateManager::HandleKeyInputs(int keyInput[])
{
	switch (m_currentState)
	{
	case State::Menu:

		m_menu.HandleKeyInputs(keyInput, m_settings);
		break;
	case State::Game:

		m_game.HandleKeyInputs(keyInput, m_settings);
		break;
	}
}

void StateManager::HandleMouseInputs(SDL_MouseButtonEvent mouseEvent)
{
	switch (m_currentState)
	{
	case State::Menu:

		m_menu.HandleMouseInputs(mouseEvent, m_buttonManager, m_settings, mousePos);
		break;
	case State::Game:

		m_game.HandleMouseInputs(mouseEvent, m_buttonManager, m_settings, mousePos);
		break;
	}
}

StateManager::StateManager()
{

	//Menu Buttons
	m_buttonManager.AddButton("Play", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 - 200 }, { 200, 100 }, [&]() {m_currentState = State::Game; });
	m_buttonManager.AddButton("MenuSettings", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 }, { 200, 100 }, [&]() {m_menu.currentSubState = MenuState::Settings; });
	m_buttonManager.AddButton("Credits", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 + 200 }, { 200, 100 }, [&]() {m_menu.currentSubState = MenuState::Credits; });
	m_buttonManager.AddButton("Main", { 200 ,100 }, { 200, 100 }, [&]() {m_menu.currentSubState = MenuState::Main; });
	m_buttonManager.AddButton("Exit", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 + 400}, { 200, 100 }, [&]() { sdl.quit = true; });

	//Game Buttons
	m_buttonManager.AddButton("MainMenu", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 }, { 200, 100 }, [&]() {m_currentState = State::Menu; m_game.Reset(); });
	m_buttonManager.AddButton("Pause", { 200 ,100 }, { 200, 100 }, [&]() {m_game.currentSubState = GameState::Pause; });
	m_buttonManager.AddButton("Resume", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 }, { 200, 100 }, [&]() {m_game.currentSubState = GameState::Unpause; });
	m_buttonManager.AddButton("GameSettings", { sdl.WIDTH / 2 ,sdl.HEIGHT / 2 }, { 200, 100 }, [&]() {m_game.currentSubState = GameState::Settings; });

	//Settings Buttons
	m_buttonManager.AddButton("Graphics", { sdl.WIDTH * (1.0f / 5.0f) , 100 }, { 150, 100 }, [&]() {m_settings.currentSubState = SettingsState::Graphics; });
	m_buttonManager.AddButton("Audio", { sdl.WIDTH * (2.0f / 5.0f) , 100 }, { 150, 100 }, [&]() {m_settings.currentSubState = SettingsState::Audio; });
	m_buttonManager.AddButton("Controls", { sdl.WIDTH * (3.0f / 5.0f), 100 }, { 150, 100 }, [&]() {m_settings.currentSubState = SettingsState::Controls; });

}

void StateManager::Update()
{
	switch (m_currentState)
	{
	case State::Menu:

		if (m_menu.currentSubState == MenuState::Main)
			m_menu.Update();
		break;
	case State::Game:

		if (m_game.currentSubState == GameState::Unpause)
		{
			//In the future, we may not want to update particles on every update call
			//Currently lacking an indicator as to when we want to update particles
			m_game.UpdateParticles();

			m_game.Update();
		}

		break;
	}

	SDL_GetMouseState(&mousePos.first, &mousePos.second);
	if (mousePos != oldmousepos)
		//std::cout << mousePos.first << ", " << mousePos.second << std::endl;

	oldmousepos = mousePos;
}

void StateManager::HandleInputs()
{
	while (SDL_PollEvent(&sdl.sdlEvent))
	{
		int keyInput[256]{ 0 };

		switch (sdl.sdlEvent.type)
		{
		case SDL_QUIT: //User requests quit
			sdl.quit = true;
			break;
		case SDL_KEYDOWN: //Reads that a key has been pressed
			keyInput[sdl.sdlEvent.key.keysym.scancode] = true;
			HandleKeyInputs(keyInput);
			break;
		case SDL_KEYUP: //Reads that a key has been released
			keyInput[sdl.sdlEvent.key.keysym.scancode] = false;
			break;
		case SDL_MOUSEBUTTONDOWN: //Reads that a mouse button has been pressed
			HandleMouseInputs(sdl.sdlEvent.button);
			break;
		case SDL_MOUSEBUTTONUP: //Reads that a mouse button has been released

			break;
		}
	}
}
