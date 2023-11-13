#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Menu
{

	void Setup();

	void Input();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload();
};