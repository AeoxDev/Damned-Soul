#pragma once
#include "StateEnums.h"

struct Settings
{
	SettingsState currentSubState = SettingsState::Graphics;
	std::pair<int, int> resolution = { sdl.WIDTH,  sdl.HEIGHT };

	void ReadKeyInputs(int[]);
	void ReadKeyOutputs(int[]);

	void ReadMouseInputs(SDL_MouseButtonEvent, ButtonManager, std::pair<int, int>);
	void ReadMouseOutputs(SDL_MouseButtonEvent, ButtonManager, std::pair<int, int>);

};