#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "Model.h"
#include "UIComponents.h"
#include "States\StateManager.h"
#include "EventFunctions.h"

void LoadLevel4()
{
	float redAdd = 0.15f;
	float greenAdd = 0.0f;
	float blueAdd = 0.0f;
	float redMult = 1.5f;
	float greenMult = 1.1f;
	float blueMult = 0.75f;

	//EntityID dog = registry.CreateEntity();
	EntityID stage = SetUpStage(redMult, greenMult, blueMult, redAdd, greenAdd, blueAdd, 1.f); //registry.CreateEntity();
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//**************************************************
	EntityID tempBoss = SetupEnemy(EnemyType::tempBoss, 20.f, 0.f, 0.f);
	Stage4IntroScene(tempBoss, 0);

	TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
	stc->scaleX = 1.0f;
	stc->scaleZ = 1.0f;


	PointOfInterestComponent poic;
	poic.weight = 10.0f;



	


	EntityID mouse = registry.CreateEntity();
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	SetDirectionLight(1.1f, 1.0f, .9f, -1.6f, -2.0f, 1.0f);
	CreatePointLight(stage, 0.65f, 0.55f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, 0.38f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.38f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.38f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.38f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	const int nrHazards = 4;
	for (size_t i = 0; i < nrHazards; i++)
	{
		SetUpHazard(HAZARD_LAVA, 1.f, redAdd, greenAdd, blueAdd, redMult, greenMult, blueMult, 1.5f);
		/*bool succeded = false;
		while (!succeded)
		{
			float randX = (float)(rand() % 64) - 32.0f;
			float randZ = (float)(rand() % 64) - 32.0f;
			if (randX * randX + randZ * randZ > 80)
			{
				float randScaleX = 16.0f + (float)((rand() % 100) * 0.1f);
				float randScaleZ = 16.0f + (float)((rand() % 100) * 0.1f);
				EntityID hazard = registry.CreateEntity();
				ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
				hazardModel->shared.colorAdditiveRed = redAdd;
				hazardModel->shared.colorAdditiveGreen = greenAdd;
				hazardModel->shared.colorAdditiveBlue = blueAdd;
				hazardModel->shared.colorMultiplicativeRed = redMult;
				hazardModel->shared.colorMultiplicativeGreen = greenMult;
				hazardModel->shared.colorMultiplicativeBlue = blueMult;
				hazardModel->shared.gammaCorrection = 1.5f;
				hazardModel->castShadow = false;

				TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
				hazardTransform->positionX = randX;
				hazardTransform->positionY = 0.5f;
				hazardTransform->positionZ = randZ;
				hazardTransform->scaleX = randScaleX;
				hazardTransform->scaleY = 1.0f;
				hazardTransform->scaleZ = randScaleZ;
				hazardTransform->facingX = cosf((float)rand());
				hazardTransform->facingZ = sinf((float)rand());
				AddStaticHazard(hazard, HAZARD_LAVA);

				succeded = true;
			}
		}*/
	}
	stateManager.stage = stage;
	RenderGeometryIndependentCollision(stage);
	SetInPlay(true);
}