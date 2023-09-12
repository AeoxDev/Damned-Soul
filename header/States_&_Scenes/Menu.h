#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Menu
{
	MenuState currentSubState = MenuState::Main;

	void Update();

	void HandleKeyInputs(int keyInput[], Settings& settings);

	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos);

};