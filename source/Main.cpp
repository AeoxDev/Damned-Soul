// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "ConfigManager.h"
//Uncomment this line for tests:
//#define TEST3000

#ifdef TEST3000
#include "UI/UIButtonFunctions.h" //Uncomment if you wanna do the funny stress-test thing
#endif // TEST


void UpdateDebugWindowTitle(std::string& title);

int main(int argc, char* args[])
{
	InitiateConfig();
	MemLib::createMemoryManager();

	bool sdlLoaded = SetupWindow();
	if (sdlLoaded == false)
	{
		return -1;
	}
	std::string title = "Damned Soul";
	if (stateManager.Setup() < 0)
	{
		return -1;
	}
	
	//Reload stress-test
#ifdef TEST3000
	for (unsigned int i = 0; i < 3000; ++i)
	{
		UIFunc::LoadNextLevel(nullptr);
		CountDeltaTime();

		UpdateDebugWindowTitle(title);//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		stateManager.Update();//Lastly do the cpu work

		stateManager.EndFrame();

		MemLib::pdefrag();
	}
#endif // TEST3000



	while (!sdl.quit)
	{
		CountDeltaTime();
		
		UpdateDebugWindowTitle(title);//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		
		stateManager.Update();//Lastly do the cpu work

		stateManager.EndFrame();

		MemLib::pdefrag();
	}
	stateManager.UnloadAll();
	SDL_Quit();
	MemLib::destroyMemoryManager();
	return 0;
}

void UpdateDebugWindowTitle(std::string& title)
{
//#ifdef _DEBUG
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
//#endif // _DEBUG Debugging purposes, not compiled in release
}