#pragma once

#include "Relics/RelicFunctions.h"
#include "MemLib/ML_Vector.hpp"

namespace UIFunc
{
	// Start the next level
	void LoadNextLevel(void* args, int a);

	// Go from start menu to play
	void MainMenu_Start(void* args, int a);

	// Go from start menu to settings
	void MainMenu_Settings(void* args, int a);

	// Quit the game from the start menu
	void MainMenu_Quit(void* args, int a);

	// Resume game from pause state
	void PauseState_ResumeGame(void* args, int a);

	// Return to main menu from pause state
	void PauseState_MainMenu(void* args, int a);

	// Go back from the settings menu to the main menu
	void Settings_Back(void* args, int a);

	// Change to low Resolution from settings
	void Settings_LowRes(void* args, int a);
	
	// Change to medium Resolution from settings
	void Settings_MediumRes(void* args, int a);

	// Change to high Resolution from settings
	void Settings_HighRes(void* args, int a);

	// Change to fullscreen from settings
	void Settings_Fullscreen(void* args, int a);

	void SelectRelic(void* args, int index);

	void BuyRelic(void* args, int index);

	// Lock a relic in shop
	void LockRelic(void* args, int index);

	// Re-roll a relic in shop
	void RerollRelic(void* args, int index);

	void EmptyOnClick(void* args, int index);

	void EmptyOnHover(void* args, int index, bool hover);

	// Heal the player
	void HealPlayer(void* args, int index);

	void HoverImage(void* args, int index, bool hover);

	void HoverShopRelic(void* args, int index, bool hover);

	void HoverPlayerRelic(void* args, int index, bool hover);
}
