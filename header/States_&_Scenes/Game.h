#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct GameScene
{
	Registry m_registry;
	
	//SceneManager sceneManager;

	//std::pair<float, float> playerPosition = { 0, 0 };
	//float playerPosition[3];//Change to vec3
	//int playerDirX = 0;
	//int playerDirY = 0;

	void Setup(int scene);

	void Clear();
	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	void Unload();
};