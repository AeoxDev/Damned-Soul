#pragma once

struct GameScene
{

	void Input(bool isShop = false);
	void Update();
	void ComputeShaders();

	void Unload(bool unloadPersistent = false);
	void GameOver();
};