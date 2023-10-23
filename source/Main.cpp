// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "ConfigManager.h"
//#include "UI/UIButtonFunctions.h" //Uncomment if you wanna do the funny stress-test thing

void UpdateDebugWindowTitle(std::string& title);

int main(int argc, char* args[])
{
	InitiateConfig();
	MemLib::createMemoryManager();

	SetupWindow();
	std::string title = "Damned Soul";
	stateManager.Setup();
	
	//Reload stress-test

	/*for (unsigned int i = 0; i < 3000; ++i)
	{
		UIFunc::MainMenu_Start(nullptr);

		SetInMainMenu(true);
		SetInPlay(false);
		stateManager.levelScenes[0].Unload();
		stateManager.levelScenes[1].Unload();
		stateManager.menu.Setup();
	}*/

	while (!sdl.quit)
	{
		CountDeltaTime();
		
		UpdateDebugWindowTitle(title);//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		
		stateManager.Update();//Lastly do the cpu work

		stateManager.EndFrame();
	}
	stateManager.UnloadAll();
	SDL_Quit();
	MemLib::destroyMemoryManager();
	return 0;
}

void UpdateDebugWindowTitle(std::string& title)
{
#ifdef _DEBUG
	if (sdl.windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		return;
	}
	if (NewSecond())
	{
		title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
		//title+="";//Add more debugging information here, updates every second.
		SetWindowTitle(title);
	}
#endif // _DEBUG Debugging purposes, not compiled in release
}