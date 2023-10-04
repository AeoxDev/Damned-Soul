#pragma once
#include "EntityFramework.h"

struct PauseState
{
	void Setup();

	void Input();
	void Update();
	void ComputeShaders();

	void Unload();
};