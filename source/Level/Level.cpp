#include "Level.h"
#include "Levels\LevelLoader.h"
#include "Camera.h"
#include "UIRenderer.h"
#include "States\StateManager.h"
#include "RelicFunctions.h"
#include "Registry.h"
#include "Components.h"

#include "UIButtonFunctions.h"

void LoadLevel(int level)
{
	//Reset UI and camera in case camera was in weird position before.

	auto relics = Relics::GetFunctionsOfType(Relics::FUNC_ON_LEVEL_SWITCH);
	for (uint32_t i = 0; i < relics.size(); ++i)
	{
		relics[i](nullptr);
	}

	RedrawUI();
	Camera::ResetCamera();
	SetInShop(false);
	stateManager.scenes[0].Unload();
	stateManager.scenes[1].Unload();
	stateManager.scenes[2].Unload();

	SetInShop(false);
	
	for (auto entity : View<ControllerComponent>(registry))
		registry.GetComponent<ControllerComponent>(entity)->enabled *= -1;

	stateManager.activeLevelScene = (stateManager.activeLevelScene + 1) % 3;
	
	switch (level)
	{
	case 1:	LoadLevel1(); break;
	case 2: LoadShop(); break;
	case 3: LoadLevel2(); break;
	case 4: LoadShop(); break;
	case 5: LoadLevel3(); break;
	case 6: LoadShop(); break;
	case 7: LoadLevel4(); break;
	default: 
		UnloadEntities(ENT_PERSIST_PLAYER);//Reset game
		stateManager.menu.Setup();
		stateManager.activeLevelScene = 0;
	}
}