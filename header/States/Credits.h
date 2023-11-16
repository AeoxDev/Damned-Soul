#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Credits
{

	void Setup();

	void Input();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload();
};