// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "ConfigManager.h"
#include "TransformComponent.h"

#include "MemLib/ML_String.hpp"
#include "DebugTests.h" // <-- ALL TEST ARE IN HERE


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

#ifdef GAME_TEST

	SimulateGame(title, 1, 1500);

#endif // GAME_TEST
	
#ifdef UI_TEST

	SimulateUI(title, 200);

#endif // UI_TEST

#ifdef MAIN_MENU_TEST

	SimulateMainMenu(title, 1000);

#endif // MAIN_MENU_TEST

#ifdef PARTICLE_TEST

	SimulateParticleLevel(title, 100);

#endif // MAIN_MENU_TEST

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