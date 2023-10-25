#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Menu
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