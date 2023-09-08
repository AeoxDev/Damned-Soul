#include "Interface\StateManager.h"

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
	if (m_currentState == State::Game && m_game.currentSubState == GameState::Unpause)
		m_game.Update();

	SDL_GetMouseState(&mousePos.first, &mousePos.second);
	if (mousePos != oldmousepos)
		//std::cout << mousePos.first << ", " << mousePos.second << std::endl;

	oldmousepos = mousePos;
}

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
