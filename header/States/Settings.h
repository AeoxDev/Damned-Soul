#pragma once
#include "StateEnums.h"

struct SettingsState
{
	void Setup();

	void Input();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload();
};