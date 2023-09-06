// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "SDLhandler.h"
#include "Input.h"
#include "MemLib/MemLib.hpp"
#include "EntityFramework.h"
#include "ConfigManager.h"
#include "DeltaTime.h"
#include "States.h"

int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
    
    //std::cout << "Hello World!\n";
    SetupWindow();
	//Hwnd = sdl.window
	State currentMainState = State::Menu;

	Game game;
	Menu menu;
	Settings settings;

	settings.resolution.first = 1920;
	settings.resolution.second = 1080;

	while (!sdl.quit)
	{
		CountDeltaTime();

		//Update
		HandleStateInput(currentMainState, game, menu, settings);

		
		//Draw

		SDL_RenderClear(sdl.sdlRenderer);
		SDL_RenderPresent(sdl.sdlRenderer);
		MemLib::pdefrag();
	}

	MemLib::destroyMemoryManager();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
