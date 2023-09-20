#pragma once
#include "StateEnums.h"
#include "Settings.h"

struct Game
{
	GameState currentSubState = GameState::Unpause;
	SceneManager sceneManager;

	//std::pair<float, float> playerPosition = { 0, 0 };
	float playerPosition[3];//Change to vec3
	int playerDirX = 0;
	int playerDirY = 0;

	void Update();
	void UpdateParticles();

	void ReadKeyInputs(int[], Settings&);
	void ReadKeyOutputs(int[], Settings&);

	void ReadMouseInputs(SDL_MouseButtonEvent, ButtonManager, Settings&, std::pair<int, int>);
	void ReadMouseOutputs(SDL_MouseButtonEvent, ButtonManager, Settings&, std::pair<int, int>);

	void Reset();
};