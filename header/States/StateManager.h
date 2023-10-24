#pragma once
//#include "ButtonManager.h"
#include "States\Menu.h"
#include "States\Game.h"
#include "States\Pause.h"
#include "Systems\Systems.h"
#include "EntityID.h"
#include <vector>

//#include "MemLib\ML_Vector.hpp"

struct StateManager;

extern State currentStates;
extern StateManager stateManager;

struct StateManager
{
	std::vector<System*> systems;
	int activeLevel = -1;
	int activeLevelScene = -1;//Current loaded level scene in array
	GameScene scenes[3];
	Menu menu;
	SettingsState settings;
	PauseState pause;
	EntityID player;
	/// <summary>
	/// StateManager.Setup is only called once, keep this in mind
	/// </summary>
	int Setup();

	void Input();//All input based cpu systems here
	void Update();//All cpu systems here
	void ComputeShaders();//All compute shaders here

	void UnloadAll();
	void EndFrame();//Clears all framebased variables.

	GameScene& GetCurrentLevel();
};