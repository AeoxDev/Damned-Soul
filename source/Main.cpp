// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "SDLhandler.h"
#include "Input.h"
#include "MemLib/MemLib.hpp"
#include "EntityFramework.h"
#include "ConfigManager.h"
#include "DeltaTime.h"



int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
    
    //std::cout << "Hello World!\n";
    SetupWindow();
	std::string title = "Damned Soul";
	//Hwnd = sdl.window
	int i = 0;
	while (!sdl.quit)
	{
		CountDeltaTime();
		HandleInput();
#ifdef _DEBUG
		if (NewSecond())
		{
			title = "Damned Soul " + std::to_string(GetAverage()) + " (" + std::to_string(GetFPS()) + " fps)";
			//title+="";//Add more debugging information here, updates every second.
			SetWindowTitle(title);
		}
#endif // _DEBUG Debugging purposes, not compiled in release

		
		
		//Update
		
		//Draw

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
