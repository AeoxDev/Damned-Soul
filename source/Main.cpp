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
#include "ExampleMenu.h"
#include "UIRenderer.h"
#include <iostream>

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

	InitializeParticles();
	SetupParticles();

	SMP_IDX sampler = CreateSamplerState();
	SetSamplerState(sampler);

	//Put into scne
	Registry collisionRegistry;
	EntityID player = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, player);
	int circle = CreateHitbox(collisionRegistry, player, 1.0f, 0.0f, 0.0f);
	SetHitboxIsPlayer(collisionRegistry, player, circle);
	SetHitboxHitEnemy(collisionRegistry, player, circle);
	float triangleX[3] = { 0.f, 1.f, 0.5f };
	float triangleZ[3] = { 0.f, 0.f, 1.f };
	int triangle = CreateHitbox(collisionRegistry, player, 3, triangleX, triangleZ);
	SetHitboxIsPlayer(collisionRegistry, player, triangle);
	SetHitboxHitEnemy(collisionRegistry, player, triangle);

	float convexPentaX[5]{ 0.5f, 1.5f, 1.5f, 1.0f, 0.5f };
	float convexPentaZ[5]{ -0.5f, -0.5f, .5f, 1.f, .5f };
	
	EntityID enemy1 = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, enemy1);
	int circle2 = CreateHitbox(collisionRegistry, enemy1, 1.0f, 1.0f, 1.0f);
	SetHitboxIsEnemy(collisionRegistry, enemy1, circle2);
	SetHitboxHitPlayer(collisionRegistry, enemy1, circle2);
	int enemyConvex = CreateHitbox(collisionRegistry, enemy1, 5, convexPentaX, convexPentaZ);
	SetHitboxIsEnemy(collisionRegistry, enemy1, enemyConvex);
	SetHitboxHitPlayer(collisionRegistry, enemy1, enemyConvex);

	UpdatePhysics(collisionRegistry);

	//EntityID enemy2 = collisionRegistry.CreateEntity();
	//AddHitboxComponent(collisionRegistry, enemy2);
	//int circle3 = CreateHitbox(collisionRegistry, enemy2, 1.0f, 2.0f, 2.0f);
	//SetHitboxIsEnemy(collisionRegistry, enemy2, circle3);
	//SetHitboxHitPlayer(collisionRegistry, enemy2, circle3);
	StateManager stateManager; //Outside of memlib at the moment, might fix later if necessary.

	if (!SetupUIRenderer())
	{
		std::cout << "Failed to setup UI Renderer" << std::endl;
		return -1;
	}

	// Create UI + Example Menu
	PoolPointer<UI> ui = MemLib::palloc(sizeof(UI));
	*ui = UI();
	PoolPointer<ExMenu> exMenu = MemLib::palloc(sizeof(ExMenu));
	exMenu->Setup(ui);

	
	ui->SetCurrentCanvas(exMenu);
	UpdateUI(ui);


	
	while (!sdl.quit)
	{

		CountDeltaTime();

		//Clear the render targets!
		Clear(testRenderSlot);
		RenderUI();

		//Inputs: SDL readings of keyboard and mouse inputs
		stateManager.HandleInputs();

		//Prepare the particles to be dispatched
		PrepareParticles();

		//Dispatch the particles
		RenderParticles(100, 1, 1, 100);

		//Finish handling the data
		FinishParticles();

		//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		//UpdatePhysics(stateManager);//Change registry to scene registry
		UpdateDebugWindowTitle(title);

		stateManager.Update();

		//Present what was drawn during the update!
		Present();
		MemLib::pdefrag();
	}
	
	ui->Release();
	
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