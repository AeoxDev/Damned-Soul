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
	void LoadNextLevel(void* args);

	void LoadParticleLevel(void* args);

	// Go from start menu to settings
	void MainMenu_Settings(void* args);

	// Quit the game from the start menu
	void MainMenu_Quit(void* args);

	// Resume game from pause state
	void PauseState_ResumeGame(void* args);

	// Return to main menu from pause state
	void PauseState_MainMenu(void* args);

	// Go back from the settings menu to the main menu
	void Settings_Back(void* args);

	// Change to low Resolution from settings
	void Settings_LowRes(void* args);
	
	// Change to medium Resolution from settings
	void Settings_MediumRes(void* args);

	// Change to high Resolution from settings
	void Settings_HighRes(void* args);

	// Change to fullscreen from settings
	void Settings_Fullscreen(void* args);

	// Buy a relic in shop
	void Shop_BuyRelic(void* args);

	// Lock a relic in shop
	void Shop_LockRelic(void* args);

	// Re-roll a relic in shop
	void Shop_ReRollRelic(void* args);

	// Heal the player
	void Shop_Heal(void* args);
}
