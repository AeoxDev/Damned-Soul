#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"
#include "Registry.h"

struct Menu
{
	EntityID mainMenuPage{-1, 0};
	//Load all main menu related
	void Setup();

	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void DrawUi();

	//Unload everything in the registry, do this after turning the menu state off.
	void Unload();
};