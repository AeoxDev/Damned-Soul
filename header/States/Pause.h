#pragma once

struct PauseState
{
	void Setup();

	void Input();

	void SetupUI();

	void Unload(int unloadPersistent = 0);
};