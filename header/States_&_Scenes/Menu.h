#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct Menu
{
	Registry registry;//This contains all entities and components for the main menu.

	//Load all main menu related
	void Setup();

	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	//Unload everything in the registry, do this after turning the menu state off.
	void Unload();
};