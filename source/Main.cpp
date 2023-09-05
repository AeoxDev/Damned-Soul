// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include "SDLhandler.h"
#include "Input.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"



int main(int argc, char* args[])
{
	MemLib::createMemoryManager();

    std::chrono::steady_clock::time_point time;
    std::chrono::steady_clock::time_point end;
    std::chrono::duration<float> deltaTimeCount;
	float deltaTime;
    //std::cout << "Hello World!\n";
    SetupWindow();
	//Hwnd = sdl.window

	SetupDirectX(sdl.window);

	int8_t testPixelShader = LoadPixelShader("bin\\TestPS.cso");
	int8_t testVertexShader = LoadVertexShader("bin\\TestVS.cso");

	int i = 0;
	while (!sdl.quit)
	{
		end = std::chrono::steady_clock::now();
		deltaTimeCount = end - time;

		time = std::chrono::steady_clock::now();
		deltaTime = deltaTimeCount.count();
		if (deltaTimeCount.count() > DELTACAP)
		{
			deltaTime = DELTACAP;
		}
		HandleInput();

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
