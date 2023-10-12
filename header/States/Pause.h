#pragma once
#include "EntityFramework.h"

struct PauseState
{
	bool m_active = false;

	void Setup();

	void Input();
	void Update();
	void ComputeShaders();

	void Unload();
};