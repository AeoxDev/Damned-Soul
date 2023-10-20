#pragma once

struct GameScene
{
	// Wether or not the scene is active
	bool m_active = false;

	void Setup(int scene);

	void Input();
	void Update();
	void ComputeShaders();

	void Unload(bool last);
	void GameOver();
};