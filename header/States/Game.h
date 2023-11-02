#pragma once

struct GameScene
{

	void Input(bool isShop = false);
	void Update();
	void ComputeShaders();

	void Unload(int unloadPersistent = 0);
	void GameOver();
};