#pragma once

struct PauseState
{
	void Setup();

	void Input();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload(bool unloadPersistent = false);
};