#include "UIButtonFunctions.h"
#include "States\StateManager.h"
#include "SDLHandler.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "Level.h"
#include "CollisionFunctions.h"

void UIFunc::MainMenu_Start(void* args)
{
	SetInPlay(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	//LoadLevel(1);
	LoadLevel(stateManager.activeLevelScene + 1);
}

void UIFunc::MainMenu_Settings(void* args)
{
	SetInSettings(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	stateManager.settings.Setup();
}

void UIFunc::MainMenu_Quit(void* args)
{
	sdl.quit = true;
}


void UIFunc::Settings_Back(void* args)
{
	SetInMainMenu(true);
	SetInSettings(false);
	stateManager.settings.Unload();
	stateManager.menu.Setup();
}
