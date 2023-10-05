#pragma once
#include "StateEnums.h"
#include "EntityFramework.h"

struct SettingsState
{
	bool m_active = false;

	void Setup();

	void Input();
	void Update();
	void ComputeShaders();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload();
};