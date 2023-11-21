#include "DebugTests.h"
#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "ConfigManager.h"
#include "UI/UIButtonFunctions.h"
#include "MemLib/ML_String.hpp"
#include "Level.h"
#include "Levels\LevelLoader.h"

void UpdateDebugWindowTitle(std::string& title, std::string extra)
{

	SetWindowTitle((title + extra).c_str());

	if (NewSecond())
	{
		title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps) ";

		SetWindowTitle((title + extra).c_str());
	}
}

#ifdef GAME_TEST

void SimulateGame(std::string& title, int sim, int total)
{
	for (unsigned int i = 0; i < total; ++i)
	{
		UIFunc::LoadNextLevel(nullptr, i);
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
	UIFunc::Game_MainMenu(nullptr, 0);

}

#endif	//GAME_TEST

#ifdef UI_TEST

void SimulateUI(std::string& title, int total)
{
	for (unsigned int i = 0; i < total; ++i)
	{
		UIFunc::MainMenu_Settings(nullptr, i);
		CountDeltaTime();

		//Show the amount of reloads we've done up in the window title. No real reason
		UpdateDebugWindowTitle(title, (" load: " + std::to_string(i) + " /" + std::to_string(total)).c_str());
		stateManager.Update();

		stateManager.EndFrame();

		MemLib::pdefrag();

		UIFunc::Settings_Back(nullptr, i);
	}

	gameSpeed = 1.0f;
}

#endif // UI_TEST

#ifdef MAIN_MENU_TEST

void SimulateMainMenu(std::string& title, int total)
{
	gameSpeed = 36.0f;
	LoadLevel(666);//Load the menu
	for (size_t i = 0; i < total; i++)
	{
		CountDeltaTime();
		UpdateDebugWindowTitle(title, (" load: " + std::to_string(i) + " /" + std::to_string(total)).c_str());
		stateManager.Update();
		stateManager.EndFrame();
		//MemLib::pdefrag();
	}
	gameSpeed = 1.0f;
	LoadLevel(666);//Reload the menu
}

#endif // MAIN_MENU_TEST

#ifdef PARTICLE_TEST

void SimulateParticleLevel(std::string& title, int total)
{
	LoadParticleLevel();
	for (size_t i = 0; i < total; i++)
	{
		CountDeltaTime();
		UpdateDebugWindowTitle(title, (" load: " + std::to_string(i) + " /" + std::to_string(total)).c_str());
		stateManager.Update();
		stateManager.EndFrame();
		//MemLib::pdefrag();
	}
	gameSpeed = 1.0f;
	UIFunc::Game_MainMenu(nullptr, 0);
}

#endif // PARTICLE_TEST

