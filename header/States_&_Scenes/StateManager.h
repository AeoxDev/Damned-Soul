#pragma once
#include "SDLHandler.h"
#include "StateEnums.h"
#include "ButtonManager.h"
#include "States_&_Scenes\Menu.h"
#include "States_&_Scenes\Game.h"

extern State currentStates;

struct StateManager
{
	GameScene levelScenes[2];
	GameScene shop;
	Menu m_menu;
	//MainMenu = 0b1,//Is a registry
	//Pause = 0b100,//Is a registry
	//Settings = 0b1000//Is a registry
	//Settings m_settings;

	//ButtonManager m_buttonManager;

	//int m_keyInput[256]{ 0 };

	//void ReadKeyInputs(int[]);
	//void ReadKeyOutputs(int[]);

	//void ReadMouseInputs(SDL_MouseButtonEvent);
	//void ReadMouseOutputs(SDL_MouseButtonEvent);

	void Setup();
	void Clear();
	void Render();
	void Update();
	void ReadInputs();


};