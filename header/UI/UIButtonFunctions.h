#pragma once

namespace UIFunc
{
	// Go from start menu to game
	void MainMenu_Start(void* args);

	// Go from start menu to settings
	void MainMenu_Settings(void* args);

	// Quit the game from the start menu
	void MainMenu_Quit(void* args);

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
}
