#pragma once

struct GameScene
{
	// Wether or not the scene is active
	bool m_active = false;

	void Input(bool isShop = false);
	void Update();
	void ComputeShaders();

	void Unload(bool unloadPersistent = false);
	void GameOver();
};