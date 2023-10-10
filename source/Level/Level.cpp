#include "Level.h"
#include "Levels\LevelLoader.h"
#include "Camera.h"
#include "UIRenderer.h"
#include "States\StateManager.h"

void LoadLevel(int level)
{
	//Reset UI and camera in case camera was in weird position before.

	RedrawUI();
	Camera::ResetCamera();

	stateManager.levelScenes[0].Unload();
	stateManager.levelScenes[1].Unload();
	stateManager.activeLevelScene = (stateManager.activeLevelScene + 1) % 2;
	stateManager.levelScenes[stateManager.activeLevelScene].Setup(1);
	switch (level)
	{
	case 1:	LoadLevel1(); break;
	case 2: LoadLevel2(); break;
	default: break;
	}
}