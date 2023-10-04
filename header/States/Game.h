#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct GameScene
{
	void Setup(int scene);

	void Input();
	void Update();
	void ComputeShaders();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void Unload();
};