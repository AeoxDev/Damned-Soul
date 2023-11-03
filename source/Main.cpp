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
#define SIMULATED_FRAMES 1
#include "UI/UIButtonFunctions.h" //Uncomment if you wanna do the funny stress-test thing
#include "Level.h"
#endif // TEST


void UpdateDebugWindowTitle(std::string& title, std::string extra);

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
		for (size_t j = 0; j < SIMULATED_FRAMES; j++)
		{
			CountDeltaTime();

			UpdateDebugWindowTitle(title, " load: " + std::to_string(i) + " / 3000");
			stateManager.Update();

			stateManager.EndFrame();

			MemLib::pdefrag();
		}
		
	}
	//Simulate main menu for 3000 frames
	gameSpeed = 24.0f;
	LoadLevel(666);//Load the menu
	for (size_t i = 0; i < 30000; i++)
	{
		CountDeltaTime();
		UpdateDebugWindowTitle(title, " frame: " + std::to_string(i) + " / 30 000");
		stateManager.Update();
		stateManager.EndFrame();
		MemLib::pdefrag();
	}
	gameSpeed = 1.0f;
	LoadLevel(666);//Reload the menu
#endif // TEST3000

	while (!sdl.quit)
	{
		CountDeltaTime();
		
		UpdateDebugWindowTitle(title, "");//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		
		stateManager.Update();//Lastly do the cpu work

		stateManager.EndFrame();

		MemLib::pdefrag();
	}
	stateManager.UnloadAll();
	SDL_Quit();
	MemLib::destroyMemoryManager();
	return 0;
}

void UpdateDebugWindowTitle(std::string& title, std::string extra)
{
//#ifdef _DEBUG
	if (sdl.windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		return;
	}
	SetWindowTitle(title + extra);
	if (NewSecond())
	{
		title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
		//title+="";//Add more debugging information here, updates every second.
		SetWindowTitle(title + extra);
	}
//#endif // _DEBUG Debugging purposes, not compiled in release
}