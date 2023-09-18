#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Menu
{
	MenuState currentSubState = MenuState::Main;

	void Update();

	void ReadKeyInputs(int[], Settings&);
	void ReadKeyOutputs(int[], Settings&);

	void ReadMouseInputs(SDL_MouseButtonEvent, ButtonManager, Settings&, std::pair<int, int>);
	void ReadMouseOutputs(SDL_MouseButtonEvent, ButtonManager, Settings&, std::pair<int, int>);

};