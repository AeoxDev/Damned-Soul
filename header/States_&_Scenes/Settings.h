#pragma once
#include "StateEnums.h"

struct Settings
{
	SettingsState currentSubState = SettingsState::Graphics;
	std::pair<int, int> resolution = { sdl.WIDTH,  sdl.HEIGHT };

	void HandleKeyInputs(int keyInput[]);

	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, std::pair<int, int> mousePos);

};