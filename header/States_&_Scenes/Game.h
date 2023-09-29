#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct GameScene
{
	
	void Setup(int scene);

	void Clear();
	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	void Unload();
};