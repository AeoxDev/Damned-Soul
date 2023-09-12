#pragma once
#include <string>
#include <iostream>

#include "SceneManager.h"
#include "ButtonManager.h"

enum class State {
	Menu,
	Game
};

enum class MenuState {
	Main,
	Settings,
	Credits
};

enum class GameState {
	Unpause,
	Pause,
	Settings
};

enum class SettingsState {
	Graphics,
	Audio,
	Controls,
};
