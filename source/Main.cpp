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
#include "UIRenderer.h"
#include "States_&_Scenes\StateManager.h"
#include <iostream>

void UpdateDebugWindowTitle(std::string& title);

int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
	SetupWindow();
	std::string title = "Damned Soul";
	Setup3dGraphics();
	ui.RenderSlot = SetupUIRenderState();

	ui.Setup();

	int testRenderSlot = SetupGameRenderer();
	Camera::InitializeCamera();
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_VERTEX);

	Particles::InitializeParticles();
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY);

	SMP_IDX sampler = CreateSamplerState();
	SetSamplerState(sampler);

	SetupTestHitbox();

	stateManager.Setup();

	currentStates = State::InMainMenu;
	
	while (!sdl.quit)
	{
		CountDeltaTime();

		//Clear the render targets!
		Clear(testRenderSlot);

		//First do compute shader work
		stateManager.ComputeShaders();

		//Then render all registries that are active
		stateManager.Render();

		//Inputs: SDL readings of keyboard and mouse inputs
		
		//Do all systems that are based on input
		stateManager.Input();

		//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		//UpdatePhysics(stateManager);//Change registry to scene registry
		UpdateDebugWindowTitle(title);

		//Lastly do the cpu work
		stateManager.Update();
		//UpdatePhysics(sceneRegistry);//Use the registry of the scene
		//Present what was drawn during the update!
		Present();
		MemLib::pdefrag();
		stateManager.EndFrame();
	}
	
	stateManager.UnloadAll();
	ReleaseUIRenderer();
	
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