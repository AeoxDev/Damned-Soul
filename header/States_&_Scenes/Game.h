#pragma once
#include "StateEnums.h"
#include "Settings.h"
#include "EntityFramework.h"

struct GameScene
{
	Registry registry;

	void Setup(int scene);

	void Clear();
	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	void SetupButtons();
	void SetupImages();
	void SetupText();

	void DrawUi();

	void Unload();
};