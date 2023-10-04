#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct Menu
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