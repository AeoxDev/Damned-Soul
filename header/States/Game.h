#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct GameScene
{
	// Wether or not the scene is active
	bool m_active = false;

	void Setup(int scene);

	void Input();
	void Update();
	void ComputeShaders();

	void Unload();
	void GameOver();
};