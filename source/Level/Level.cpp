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

	stateManager.scenes[0].Unload();
	stateManager.scenes[1].Unload();
	stateManager.scenes[2].Unload();
	
	stateManager.activeLevelScene = (stateManager.activeLevelScene + 1) % 3;
	stateManager.scenes[stateManager.activeLevelScene].m_active = true;
	
	switch (level)
	{
	case 1:	LoadLevel1(); break;
	case 2: LoadShop(); break;
	case 3: LoadLevel2(); break;
	default: stateManager.menu.Setup();
	}
}