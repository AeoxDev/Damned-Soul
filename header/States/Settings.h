#pragma once
#include "StateEnums.h"
#include "EntityFramework.h"

struct SettingsState
{
	void Setup();

	void Input();
	void Update();
	void ComputeShaders();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload();
};