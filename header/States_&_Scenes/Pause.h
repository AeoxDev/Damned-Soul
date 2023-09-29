#pragma once
#include "EntityFramework.h"

struct PauseState
{

	void Setup();

	void Clear();
	void ComputeShaders();
	void Render();
	void Input();
	void Update();

	void Unload();
};