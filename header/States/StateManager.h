#pragma once
//#include "ButtonManager.h"
#include "States\Menu.h"
#include "States\Game.h"
#include "States\Pause.h"
#include "States\Credits.h"
#include "Systems\Systems.h"
#include "EntityID.h"

//#include "MemLib\ML_Vector.hpp"

#include <vector>

struct StateManager;

extern State currentStates;
extern StateManager stateManager;

struct StateManager
{
	//ML_Vector<System*> systems; //Herman Help;
	std::vector<System*> systems;
	int activeLevel = -1;
	int activeLevelScene = -1;//Current loaded level scene in array
	GameScene scenes[3] = {};
	Menu menu;
	SettingsState settings;
	PauseState pause;
	Credits credits;
	EntityID player;
	EntityID stage;


	/// <summary>
	/// StateManager.Setup is only called once, keep this in mind
	/// </summary>
	int Setup();

	void Input();//All input based cpu systems here
	void Update();//All cpu systems here

	void UnloadAll();
	void EndFrame();//Clears all framebased variables.

	GameScene& GetCurrentLevel();
};