#pragma once
//#include "ButtonManager.h"
#include "States\Menu.h"
#include "States\Game.h"
#include "States\Pause.h"
#include "Systems\Systems.h"
#include <vector>

//#include "MemLib\ML_Vector.hpp"


struct StateManager;

extern State currentStates;
extern StateManager stateManager;

struct StateManager
{
	std::vector<System*> systems;
	int activeLevel = 0;
	int activeLevelScene = 0;//Current loaded level scene in array
	GameScene levelScenes[2];
	GameScene shop;
	Menu menu;
	SettingsState settings;
	PauseState pause;

	/// <summary>
	/// StateManager.Setup is only called once, keep this in mind
	/// </summary>
	void Setup();

	void Input();//All input based cpu systems here
	void Update();//All cpu systems here
	void ComputeShaders();//All compute shaders here

	void UnloadAll();
	void EndFrame();//Clears all framebased variables.

	GameScene GetCurrentLevel();
};