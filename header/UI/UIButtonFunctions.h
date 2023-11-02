#pragma once

#include "Relics/RelicFunctions.h"
#include "MemLib/ML_Vector.hpp"

//enum temprelicslist
//{
//	DemonBonemarrow,
//	FlameWeapon,
//	SoulPower,
//	DemonHeart,
//	FrostFire,
//	SoulHealth,
//	SpeedyLittleDevil,
//	LightningGod
//};

namespace UIFunc
{
	// Start the next level
	void LoadNextLevel(void* args, int a);

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

	// Buy a relic in shop
	void Shop_BuyRelic(void* args, int a);

	// Lock a relic in shop
	void Shop_LockRelic(void* args, int a);

	// Re-roll a relic in shop
	void Shop_ReRollRelic(void* args, int a);

	// Heal the player
	void Shop_Heal(void* args, int a);

	void HoverImage(void* args, int index, bool hover);
}
