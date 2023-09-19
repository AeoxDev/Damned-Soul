#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Game
{
	GameState currentSubState = GameState::Unpause;
	SceneManager sceneManager;

	//std::pair<float, float> playerPosition = { 0, 0 };
	float playerPosition[3];//Change to vec3

	void Update();
	void UpdateParticles();

	void HandleKeyInputs(int keyInput[], Settings& settings);

	void HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos);

	void Reset();
};