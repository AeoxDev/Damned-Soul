#pragma once
#include "SDLHandler.h"
#include "StateEnums.h"
#include "ButtonManager.h"
#include "States_&_Scenes\Menu.h"
#include "States_&_Scenes\Game.h"
#include "States_&_Scenes\Pause.h"

struct StateManager;

extern State currentStates;
extern StateManager stateManager;

struct StateManager
{
	int activeLevelScene = 0;
	GameScene levelScenes[2];
	GameScene shop;
	Menu menu;
	SettingsState settings;
	PauseState pause;
	//MainMenu = 0b1,//Is a registry
	//Pause = 0b100,//Is a registry
	//Settings = 0b1000//Is a registry


	void Setup();
	void Clear();
	void ComputeShaders();//All compute shaders here
	void Render();//All gpu systems here
	void Input();//All input based cpu systems here
	void Update();//All cpu systems here

	void EndFrame();//Clears all framebased variables.

	void UnloadAll();


};