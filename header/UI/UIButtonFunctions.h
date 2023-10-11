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
}
