// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "ConfigManager.h"
#include "TransformComponent.h"
#include "Registry.h"
#include "UIButtonFunctions.h"
#include "Input.h"
#include "Level.h"
#include "UIRenderer.h"

#include "MemLib/ML_String.hpp"

//#define GAME_TEST
//
//#define UI_TEST
//
//#define PAUSE_TEST
//
//#define PARTICLE_TEST

//Displays info in the application title
void UpdateDebugWindowTitle(std::string& title, std::string extra = "");

#ifdef GAME_TEST

//Simulate [sim] frames in every level for [total] frames
void SimulateGame(std::string& title, int sim, int total);

#endif // GAME_TEST

#ifdef UI_TEST

//Simulate Main Menu <-> Settings state switching [total] times
void SimulateUI(std::string& title, int total);

#endif // UI_TEST

#ifdef PAUSE_TEST

//Simulate Main Menu <-> Settings state switching [total] times
void SimulatePause(std::string& title, int total);

#endif // PAUSE_TEST

#ifdef PARTICLE_TEST

void SimulateParticleLevel(std::string& title, int total);

#endif // PARTICLE_TEST


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

#ifdef PAUSE_TEST

	SimulatePause(title, 2000);

#endif // PAUSE_TEST

#ifdef PARTICLE_TEST

	SimulateParticleLevel(title, 100);

#endif // PARTICLE_TEST

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

void UpdateDebugWindowTitle(std::string& title, std::string extra)
{
	SetWindowTitle((title + extra).c_str());
	bool newSec = NewSecond();
#ifdef _DEBUG
	//Get player transform
	TransformComponent* transform = nullptr;
	if (stateManager.player.index != -1)
	{
		transform = registry.GetComponent<TransformComponent>(stateManager.player);
	}

	if (newSec)
	{
		title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps) ";
		if (transform != nullptr)
		{
			title += "(x: " + std::to_string(transform->positionX) + ", z: " + std::to_string(transform->positionZ) + ") ";
		}
		SetWindowTitle((title + extra).c_str());
	}
#endif

}


#ifdef GAME_TEST

void SimulateGame(std::string& title, int sim, int total)
{
	for (unsigned int i = 0; i < total; ++i)
	{
		if (i % 18 == 0)
		{
			UIFunctions::MainMenu::Start(nullptr, i);
		}

		UIFunctions::Game::LoadNextLevel(nullptr, i);
		for (size_t j = 0; j < sim; j++)
		{
			CountDeltaTime();

			//Show the amount of reloads we've done up in the window title. No real reason
			UpdateDebugWindowTitle(title, " load: " + std::to_string(i) + " /" + std::to_string(total));
			stateManager.Update();

			stateManager.EndFrame();

			MemLib::pdefrag();
		}
	}

	gameSpeed = 1.0f;
	UIFunctions::Game::SetMainMenu(nullptr, 0);

}

#endif	//GAME_TEST

#ifdef UI_TEST

void SimulateUI(std::string& title, int total)
{
	for (unsigned int i = 0; i < total; ++i)
	{
		UIFunctions::MainMenu::SetSettings(nullptr, i);
		CountDeltaTime();

		//Show the amount of reloads we've done up in the window title. No real reason
		UpdateDebugWindowTitle(title, (" load: " + std::to_string(i) + " /" + std::to_string(total)).c_str());
		stateManager.Update();

		stateManager.EndFrame();

		MemLib::pdefrag();

		UIFunctions::Settings::Back(nullptr, i);
	}

	gameSpeed = 1.0f;
}

#endif // UI_TEST

#ifdef PAUSE_TEST

void SimulatePause(std::string& title, int total)
{
	UIFunctions::MainMenu::Start(nullptr, 0);
	LoadLevel(-1);
	for (unsigned int i = 0; i < total; ++i)
	{
		UIFunctions::Game::SetPause(nullptr, i);
		CountDeltaTime();

		//Show the amount of reloads we've done up in the window title. No real reason
		UpdateDebugWindowTitle(title, (" load: " + std::to_string(i) + " /" + std::to_string(total)).c_str());
		stateManager.Update();

		stateManager.EndFrame();

		MemLib::pdefrag();

		UIFunctions::Pause::Resume(nullptr, i);
	}

	gameSpeed = 1.0f;
	UIFunctions::Game::SetMainMenu(nullptr, 0);
}

#endif // PAUSE_TEST

#ifdef PARTICLE_TEST

void SimulateParticleLevel(std::string& title, int total)
{
	for (size_t i = 0; i < total; i++)
	{
		LoadLevel(-2);

		for (int j = 0; j < 60; j++)
		{
			CountDeltaTime();
			UpdateDebugWindowTitle(title, (" load: " + std::to_string(i) + " /" + std::to_string(total)).c_str());

			stateManager.Update();
			stateManager.EndFrame();

			MemLib::pdefrag();
		}

		UIFunctions::Game::SetMainMenu(nullptr, 0);
	}

	gameSpeed = 1.0f;
	UIFunctions::Game::SetMainMenu(nullptr, 0);
}


#endif // PARTICLE_TEST


