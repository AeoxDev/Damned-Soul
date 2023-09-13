#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Game
{
	GameState currentSubState = GameState::Unpause;
	SceneManager sceneManager;

	std::pair<float, float> playerPosition = { 0, 0 };

	void Update();

	void HandleKeyInputs(int keyInput[], Settings& settings);

	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos);

	void Reset();
};