// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include "EntityFramework.h"
#include "ConfigManager.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Particles.h"
#include "GameRenderer.h"
#include "Hitbox.h"
#include "States_&_Scenes\StateManager.h"
#include "Model.h"
#include "ComponentHelper.h"

void UpdateDebugWindowTitle(std::string& title);

int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
    SetupWindow();
	std::string title = "Damned Soul";

	int testRenderSlot = SetupGameRenderer();
	Camera::InitializeCamera();
	SetConstantBuffer(Camera::GetCameraBufferIndex());

	Registry collisionRegistry;
	EntityID player = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, player);
	int circle = CreateHitbox(collisionRegistry, player, 1.0f, 0.0f, 0.5f);
	SetHitboxIsPlayer(collisionRegistry, player, circle);
	SetHitboxHitEnemy(collisionRegistry, player, circle);
	
	EntityID enemy1 = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, enemy1);
	int circle2 = CreateHitbox(collisionRegistry, enemy1, 1.0f, 0.0f, 1.0f);
	SetHitboxIsEnemy(collisionRegistry, enemy1, circle2);
	SetHitboxHitPlayer(collisionRegistry, enemy1, circle2);
	StateManager stateManager; //Outside of memlib at the moment, might fix later if necessary.

	EntityID enemy2 = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, enemy2);
	int circle3 = CreateHitbox(collisionRegistry, enemy2, 1.0f, 2.0f, 2.0f);
	SetHitboxIsEnemy(collisionRegistry, enemy2, circle3);
	SetHitboxHitPlayer(collisionRegistry, enemy2, circle3);

	InitializeParticles();
	SetupParticles();

	while (!sdl.quit)
	{
		CountDeltaTime();

		//Before dispatch, switch particles read and write
		//SwitchInputOutput();
		DispatchParticles(100, 1, 1);

		//Render: GPU calls. Always tell the GPU what to do first for optimal parallelism
		Render(testRenderSlot);

		//Inputs: SDL readings of keyboard and mouse inputs
		stateManager.HandleInputs();

		//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		UpdatePhysics(collisionRegistry);//Change registry to scene registry
		UpdateDebugWindowTitle(title);

		stateManager.Update();

		// Present what was drawn during the update!
		Present();
		MemLib::pdefrag();
	}

	EndDirectX();
	MemLib::destroyMemoryManager();
	SDL_Quit();
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

void UpdateDebugWindowTitle(std::string& title)
{
#ifdef _DEBUG
	if (sdl.windowFlags == 0 && NewSecond())
	{
		title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
		//title+="";//Add more debugging information here, updates every second.
		SetWindowTitle(title);
	}
#endif // _DEBUG Debugging purposes, not compiled in release
}