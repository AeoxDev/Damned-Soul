#pragma once
#include "StateEnums.h"
#include "EntityFramework.h"

struct SettingsState
{
	void Setup();

	void Clear();
	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void DrawUi();

	void Unload();
};