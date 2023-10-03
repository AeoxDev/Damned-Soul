#pragma once
#include "SDLHandler.h"
#include "StateEnums.h"
#include "ButtonManager.h"
#include "States_&_Scenes\Menu.h"
#include "States_&_Scenes\Game.h"
#include "States_&_Scenes\Pause.h"
#include "Systems\Systems.h"
#include <vector>


struct StateManager;

extern State currentStates;
extern StateManager stateManager;

struct StateManager
{
	std::vector<System*> systems;
	int activeLevelScene = 0;
	GameScene levelScenes[2];
	GameScene shop;
	Menu menu;
	SettingsState settings;
	PauseState pause;

	/// <summary>
	/// StateManager.Setup is only called once, keep this in mind
	/// </summary>
	void Setup();
	void Clear();
	void ComputeShaders();//All compute shaders here
	void Render();//All gpu systems here
	void Input();//All input based cpu systems here
	void Update();//All cpu systems here

	void EndFrame();//Clears all framebased variables.
	void UnloadAll();
};