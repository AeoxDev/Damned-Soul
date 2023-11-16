#include "Level.h"
#include "Levels\LevelLoader.h"
#include "Camera.h"
#include "UIRenderer.h"
#include "States\StateManager.h"
#include "Relics/RelicFunctions.h"
#include "Registry.h"
#include "Components.h"
#include "UIButtonFunctions.h"
#include <time.h>

void LoadLevel(int level)
{
	std::srand((unsigned)time(NULL));
	//Reset UI and camera in case camera was in weird position before.
	SetInPlay(false);
	SetInShop(false);
	SetInMainMenu(false);
	auto relics = Relics::GetFunctionsOfType(Relics::FUNC_ON_LEVEL_SWITCH);
	for (uint32_t i = 0; i < relics.size(); ++i)
	{
		relics[i](nullptr);
	}

	RedrawUI();
	Camera::ResetCamera();
	stateManager.scenes[0].Unload();
	stateManager.scenes[1].Unload();
	stateManager.scenes[2].Unload();
	//Bugfix: Reset current speed to stop sliding during shop.
	if (stateManager.player.index != -1)
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(stateManager.player);
		if (transform != nullptr)
		{
			transform->currentSpeedX = 0.0f;
			transform->currentSpeedZ = 0.0f;
		}
		
	}

	for (auto entity : View<ControllerComponent>(registry))
		registry.GetComponent<ControllerComponent>(entity)->enabled *= -1;

	stateManager.activeLevelScene = (stateManager.activeLevelScene + 1) % 3;
	Camera::SetCutsceneMode(false);
	switch (level)
	{
	case 1:	LoadLevel1(); break;
	case 2: LoadShop(); break;
	case 3: LoadLevel2(); break;//Imp stage
	case 4: ReloadShop(); break;
	case 5: LoadLevel3(); break;//Doggie stage
	case 6: ReloadShop(); break;
	case 7: LoadLevel4(); break;//Boss stage
	case 8: ReloadShop(); break;
	case 9: LoadLevel5(); break;//Eye stage
	case 10: ReloadShop(); break;//
	case 11: LoadLevel6(); break;//Minotaur stage
	case 12: ReloadShop(); break;
	case 13: LoadLevel7(); break;//Imp2 stage
	case 14: ReloadShop(); break;
	case 15: LoadLevel8(); break;//Split2 stage
	case 16: ReloadShop(); break;
	case 17: LoadLevel9(); break;//Final boss stage
	default: 
		UnloadEntities(ENT_PERSIST_LEVEL);//Reset game
		stateManager.menu.Setup();
		stateManager.activeLevelScene = 0;
	}
}