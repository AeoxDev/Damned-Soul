#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "EventFunctions.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "EntityCreation.h"
#include "DeltaTime.h"
#include "Model.h"
#include "UIComponents.h"
#include "States\StateManager.h"
#include "Input.h"
#include "Camera.h"

void LoadEmptyLevel()
{
	float redMult = 1.0f;
	float greenMult = 1.0f;
	float blueMult = 1.0f;
	StageSetupVariables stageVars;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 1;
	stageVars.scaleX = 1.0f;
	stageVars.scaleY = 1.0f;
	stageVars.scaleZ = 1.0f;
	stageVars.offsetY = -0.1f;
	//stageVars.offsetX = 16.f;
	EntityID stage = SetUpStage(stageVars); //registry.CreateEntity();
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level1"); //Proximity hitbox (Added by Joaquin)

	if (SetupVFXTorches("LV1Torch.dss", false, false) == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV1Torch\nOr file is not written properly.");
	}

	EntityID mouse = registry.CreateEntity();


	SetGISpawnPosition(-0.0f, -0.0f);
	ReloadPlayerNonGlobals();

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	EntityID timeEntity = registry.CreateEntity(ENT_PERSIST_LEVEL);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(timeEntity);
	uiElement->Setup("ButtonSmall", "Time: 0", DSFLOAT2(0.8f, 0.8f));

	if (!GetVisualTimer())
	{
		uiElement->m_BaseText.baseUI.SetVisibility(false);
		uiElement->m_BaseImage.baseUI.SetVisibility(false);
	}

	UIGameTimeComponent* runtime = registry.AddComponent<UIGameTimeComponent>(timeEntity);

	stateManager.stage = stage;
	SetInPlay(true);
}