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


enum CursorMode
{
	CursorNeutral,
	CursorDown,
	CursorSpecial
};


int GetNrEnemies();

struct StateManager
{
	std::vector<System*> systems;
	int activeLevel = -1;
	int finalLevel = 17;
	int activeLevelScene = -1;//Current loaded level scene in array
	GameScene scenes[3] = {};
	Menu menu;
	SettingsState settings;
	PauseState pause;
	Credits credits;
	EntityID player;
	EntityID weapon;
	EntityID stage;
	EntityID gate;
	EntityID gateVisual;//The visual gate
	EntityID cutsceneEnemy;
	EntityID hitboxVis;
	EntityID naviagtion;

	int creditsIndex = 0;

	EntityID cursor;
	int cursorMode = CursorNeutral;

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