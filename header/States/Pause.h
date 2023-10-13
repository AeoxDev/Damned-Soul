#pragma once

struct PauseState
{
	bool m_active = false;

	void Setup();

	void Input();
	void Update();
	void ComputeShaders();

	void Unload();
};