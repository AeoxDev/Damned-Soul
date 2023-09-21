#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct Menu
{
	Registry registry;//This contains all entities and components for the main menu.

	void SetupMainMenu();

	void MenuSystem();

	void Unload();
};