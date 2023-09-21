#pragma once
#include <string>
#include <iostream>

#include "SceneManager.h"
#include "ButtonManager.h"

//enum class State {
//	Menu,
//	Game
//};
//
//enum class MenuState {
//	Main,
//	Settings,
//	Credits
//};
//
//enum class GameState {
//	Unpause,
//	Pause,
//	Settings
//};

//Which states (Which registry/registries to have systems go over)
enum State
{
	MainMenu = 0b1,//Is a registry
	Play = 0b10,//Will look at registries
	Pause = 0b100,//Is a registry
	Settings = 0b1000//Is a registry
};

//Settings state
//enum class SettingsState {
//	Graphics,
//	Audio,
//	Controls,
//};
