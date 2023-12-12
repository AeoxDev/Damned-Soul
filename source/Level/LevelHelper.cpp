#include "Levels\LevelHelper.h"
#include "Registry.h"
#include "Components.h"
#include "Hitbox.h"
#include "Model.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "UIComponents.h"
#include "UIButtonFunctions.h"
#include "UI\HP_BarHelper.h"
#include "SDLHandler.h"
#include "DeltaTime.h"

#include "UIComponents.h"
#include "UIRenderer.h"

#include <iostream>
#include <fstream>

bool SetValueForEnemy(ModelTextRead* infoStruct, int index, std::string infoPiece) // help function for setupAllEnemies. DO NOT TOUCH
{
	if (index == 0) // enemy type
	{
		if (infoPiece == "SkeletonWeak")
		{
			infoStruct->eType = EnemyType::skeleton;
		}
		else if (infoPiece == "ImpWeak")
		{
			infoStruct->eType = EnemyType::imp;
		}
		else if (infoPiece == "HoundWeak")
		{
			infoStruct->eType = EnemyType::hellhound;
		}
		else if (infoPiece == "SkeletonStrong")
		{
			infoStruct->eType = EnemyType::empoweredSkeleton;
		}
		else if (infoPiece == "Eye")
		{
			infoStruct->eType = EnemyType::eye;
		}
		else if (infoPiece == "Minotaur")
		{
			infoStruct->eType = EnemyType::minotaur;
		}
		else if (infoPiece == "ImpStrong")
		{
			infoStruct->eType = EnemyType::empoweredImp;
		}
		else if (infoPiece == "HoundStrong")
		{
			infoStruct->eType = EnemyType::empoweredHellhound;
		}
		else if (infoPiece == "SplitBoss")
		{
			infoStruct->eType = EnemyType::tempBoss;
		}
		else if (infoPiece == "FrozenEye")
		{
			infoStruct->eType = EnemyType::frozenEye;
		}
		else if (infoPiece == "FrozenHound")
		{
			infoStruct->eType = EnemyType::frozenHellhound;
		}
		else if (infoPiece == "FrozenImp")
		{
			infoStruct->eType = EnemyType::frozenImp;
		}
		else
		{
			return false;
		}
	}
	else if (index == 1) // position x
	{
		infoStruct->positionX = std::stof(infoPiece); // converts string to float
	}
	else if (index == 2) // position z, THIS SHOULD ALWAYS BE THE NEGATIVE VALUE FROM WHAT WE ARE READING
	{
		infoStruct->positionZ = std::stof(infoPiece) * (-1); // converts string to float and multiplies with -1 because of reason above
	}
	else if (index == 3) // souls
	{
		infoStruct->soulValue = std::stoi(infoPiece); // converts string to int
	}
	else
	{
		return false;
	}
	return true;
}


bool SetValueForEnemy(VFXTextRead* infoStruct, int index, std::string infoPiece) // help function for SetupVFXTorches. DO NOT TOUCH
{
	if (index == 0) //posX
	{
		infoStruct->positionX = std::stof(infoPiece); // converts string to float
	}
	else if (index == 1) // posY
	{
		infoStruct->positionY = std::stof(infoPiece); // converts string to float
	}
	else if (index == 2) // posz
	{
		infoStruct->positionZ = std::stof(infoPiece); // converts string to float
	}
	else if (index == 3) // red
	{
		infoStruct->r = std::stof(infoPiece); // converts string to float
	}
	else if (index == 4) // green
	{
		infoStruct->g = std::stof(infoPiece); // converts string to float
	}
	else if (index == 5) // blue
	{
		infoStruct->b = std::stof(infoPiece); // converts string to float
	}
	else
	{
		return false; //something went wrong
	}
}

bool SetupAllEnemies(std::string filePath)
{
	std::string name = "EnemyMaps\\";
	name.append(filePath);

	std::ifstream myFile;
	myFile.open(name.c_str());
	std::string line = "";
	std::string term = "";
	if (myFile.is_open())
	{
		while (std::getline(myFile, line))
		{
			ModelTextRead theInfo;
			theInfo.eType = skeleton;
			theInfo.positionX = 0;
			theInfo.positionZ = 0;
			theInfo.soulValue = 69;
			int counter = 0; // by format:
			// 0 = enemyType
			// 1 = positionX
			// 2 = positionZ
			// 3 = soulCount
			for (auto t : line)
			{
				if (t == ',')
				{
					// we got the string
					if (!SetValueForEnemy(&theInfo, counter, term))
					{
						myFile.close();
						return false; // invalid values, probably type. DO NOT DO ANYTHING; 
					}
					//reset
					term = "";
					counter++;
				}
				else
				{
					term += t; //add char to string
				}
			}
			if(theInfo.soulValue != 69)
 				SetupEnemy(theInfo.eType, theInfo.positionX, 0.f, theInfo.positionZ, theInfo.soulValue);
		}
	}
	for (auto enemyEntity : View<FrozenBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		registry.RemoveComponent<FrozenBehaviour>(enemyEntity);
	}
		
	myFile.close();

	return true;
}

void SpawnTorch(float positionX, float positoinY, float positionZ, float red, float green, float blue, bool level8, bool level9)
{
	if (level8 == false && level9 == false)
	{
		EntityID particlesVFX = registry.CreateEntity();		//no,  no,    size , offset xyz
		registry.AddComponent<ParticleComponent>(particlesVFX, 100.0f, 100.0f, 7.0f, 0.0f, 1.0f, 1.0f, 32, VFX_PATTERN::FLAME);
		TransformComponent tComp;
		tComp.positionX = positionX;
		tComp.positionY = positoinY;
		tComp.positionZ = -positionZ;
		registry.AddComponent<TransformComponent>(particlesVFX, tComp);
	}
	else if (level8 == true && level9 == false)
	{
		EntityID particlesVFX = registry.CreateEntity();		//no,  no,    size , offset xyz
		registry.AddComponent<ParticleComponent>(particlesVFX, 100.0f, 100.0f, 16.0f, 0.0f, 4.0f, 2.5f, 32, VFX_PATTERN::FLAME_BLUE);
		TransformComponent tComp;
		tComp.positionX = positionX;
		tComp.positionY = positoinY;
		tComp.positionZ = -positionZ;
		registry.AddComponent<TransformComponent>(particlesVFX, tComp);
	}
	else if (level8 == false && level9 == true)
	{
		EntityID particlesVFX = registry.CreateEntity();		//no,  no,    size , offset xyz
		registry.AddComponent<ParticleComponent>(particlesVFX, 100.0f, 60.0f, 18.0f, 0.0f, 7.5f, 6.0f, 32, VFX_PATTERN::FLAME_BLUE);
		TransformComponent tComp;
		tComp.positionX = positionX;
		tComp.positionY = positoinY;
		tComp.positionZ = -positionZ;
		registry.AddComponent<TransformComponent>(particlesVFX, tComp);
	}
}



bool SetupVFXTorches(std::string filePath, bool level8, bool level9)
{
	std::string name = "VFX\\";
	name.append(filePath);

	std::ifstream myFile;
	myFile.open(name.c_str());
	std::string line = "";
	std::string term = "";
	if (myFile.is_open())
	{
		while (std::getline(myFile, line))
		{
			VFXTextRead theInfo;
			int counter = 0; // by format:
			// 0 = positionX
			// 1 = positionY
			// 2 = positionZ
			// 3 = r
			// 4 = g
			// 5 = b
			for (auto t : line)
			{
				if (t == ',')
				{
					// we got the string
					if (!SetValueForEnemy(&theInfo, counter, term))
					{
						myFile.close();
						return false; // invalid values, probably type. DO NOT DO ANYTHING; 
					}
					//reset
					term = "";
					counter++;
				}
				else
				{
					term += t; //add char to string
				}
			}
			SpawnTorch( theInfo.positionX, theInfo.positionY, theInfo.positionZ, theInfo.r, theInfo.g, theInfo.b, level8, level9);
		}
	}

	myFile.close();

	return true;
}

EntityID SetUpStage(StageSetupVariables& stageVars)
{
	EntityID stage = registry.CreateEntity();
	EntityID hitbox = registry.CreateEntity();
	EntityID gate = registry.CreateEntity();
	stateManager.naviagtion = registry.CreateEntity();
	EntityID torch = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	ModelBonelessComponent* stageModel;
	ModelBonelessComponent* hitboxModel;
	ModelBonelessComponent* gateModel;
	ModelBonelessComponent* torchModel;
	ModelBonelessComponent* portalModel;

	GlowComponent* torchGlow;
	GlowComponent* stageGlow;
	GlowComponent* portalGlow;
	
	float lavaGlowRGB[3] = { 0.9f, 0.3f, 0.0f };
	float acidGlowRGB[3] = { 0.8f, 0.9f, 0.0f };
	float iceStageGlowRGB[3] = { 0.4f, 1.0f, 1.0f };
	float iceHazardGlowRGB[3] = { 0.7f, 0.6f, 1.0f };
	float portalGlowRGB[3] = { 0.6f, 0.9f, 0.6f };

	switch (stageVars.stageNr)
	{
	case 0: 
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV1Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV1Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV1Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV1Nav.mdl"));
		torchModel = registry.AddComponent<ModelBonelessComponent>(torch, LoadModel("LV1Torch.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(0.7f, 0.7f, 0.71f, -1.6f, -3.0f, 1.0f);
		// set glow components
		torchGlow = registry.AddComponent<GlowComponent>(torch, 1, 0.6f, 0);
		stageGlow = registry.AddComponent<GlowComponent>(stage, portalGlowRGB);	// Portal glow
		break;
	case 1:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV1Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV1Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV1Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV1Nav.mdl"));
		torchModel = registry.AddComponent<ModelBonelessComponent>(torch, LoadModel("LV1Torch.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(0.7f, 0.7f, 0.71f, -1.6f, -3.0f, 1.0f);//float colorRed, float colorGreen, float colorBlue, float directionX, float directionY, float directionZ)
		// set glow components
		torchGlow = registry.AddComponent<GlowComponent>(torch, 1, 0.6f, 0);
		stageGlow = registry.AddComponent<GlowComponent>(stage, portalGlowRGB);	// Portal glow
		break;
	case 2:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV2Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV2Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV2Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV2Nav.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(0.7f, 0.7f, 0.71f, -1.6f, -3.0f, 1.0f);
		//Set glow components.
		stageGlow = registry.AddComponent<GlowComponent>(stage, portalGlowRGB);	// Portal glow
		
		break;
	case 3:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV3Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV3Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV3Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV3Nav.mdl"));
		torchModel = registry.AddComponent<ModelBonelessComponent>(torch, LoadModel("LV3Torch.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(0.71f, 0.7f, 0.7f, -1.6f, -3.0f, 1.0f);
		{
			//Add static hazards here
			EntityID hazardLava = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazardLava, LoadModel("LV3Lava.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazardLava);
			registry.AddComponent<GlowComponent>(hazardLava, lavaGlowRGB);
			AddStaticHazard(hazardLava, HAZARD_LAVA);
		}
		//Set glow components.
		torchGlow = registry.AddComponent<GlowComponent>(torch, 1, 0.6f, 0);
		stageGlow = registry.AddComponent<GlowComponent>(stage, portalGlowRGB);	// Portal glow
		
		break;
	case 4:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV4Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV4Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV4Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV4Nav.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV4Lava.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, lavaGlowRGB);
			AddStaticHazard(hazard, HAZARD_LAVA);
		}
		SetDirectionLight(1.f, 0.7f, 0.7f, -1.6f, -3.0f, 1.0f);
		//Set glow components.
		stageGlow = registry.AddComponent<GlowComponent>(stage, portalGlowRGB);	// Portal glow
		
		break;
	case 5:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV5Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV5Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV5Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV5Nav.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(1.f, 0.7f, 0.7f, -1.6f, -3.0f, 1.0f);
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV5Lava.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, lavaGlowRGB);
			AddStaticHazard(hazard, HAZARD_LAVA);
		}
		//Set glow components.
		stageGlow = registry.AddComponent<GlowComponent>(stage, portalGlowRGB);	// Portal glow
		break;
	case 6:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV6Geo.mdl"));//not finished from here
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV6Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV6Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV6Nav.mdl"));
		//hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV6Hitbox.mdl"));
		//hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV6Nav.mdl"));	//why? /Erika & Niclas
		torchModel = registry.AddComponent<ModelBonelessComponent>(torch, LoadModel("LV6Torch.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(0.61f, 0.6f, 0.62f, -1.6f, -3.0f, 1.0f);
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV6Lava.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, lavaGlowRGB);
			AddStaticHazard(hazard, HAZARD_LAVA);
		}
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV6Acid.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, acidGlowRGB);
			AddStaticHazard(hazard, HAZARD_ACID);
		}
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV6Ice.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, iceHazardGlowRGB);
			AddStaticHazard(hazard, HAZARD_ICE);
		}
		{
			//Do the visual gate here
			stateManager.gateVisual = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(stateManager.gateVisual, LoadModel("LV6GateGeo.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(stateManager.gateVisual);
		}
		//Set glow components.
		torchGlow = registry.AddComponent<GlowComponent>(torch, 1, 0.6f, 0);
		stageGlow = registry.AddComponent<GlowComponent>(stage, iceStageGlowRGB);	// Ice glow.
		portalGlow = registry.AddComponent<GlowComponent>(stateManager.gateVisual, portalGlowRGB);	// Portal glow
		break;
	case 7:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV7Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV7Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV7Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV7Nav.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		{
			//Do the visual gate here
			stateManager.gateVisual = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(stateManager.gateVisual, LoadModel("LV7GateGeo.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(stateManager.gateVisual);
		}
		{
			//Add static hazards here: ICE
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV7Ice.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, iceHazardGlowRGB);
			AddStaticHazard(hazard, HAZARD_ICE);
		}
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV7Acid.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, acidGlowRGB);
			AddStaticHazard(hazard, HAZARD_ACID);
		}
		SetDirectionLight(0.6f, 0.6f, 0.61f, -1.6f, -3.0f, 1.0f);
		//Set glow components.
		stageGlow = registry.AddComponent<GlowComponent>(stage, iceStageGlowRGB);	// Ice glow.
		portalGlow = registry.AddComponent<GlowComponent>(stateManager.gateVisual, portalGlowRGB);	// Portal glow
		break;
	case 8:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV8Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV8Gate.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV8Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV8Nav.mdl"));
		torchModel = registry.AddComponent<ModelBonelessComponent>(torch, LoadModel("LV8Torch.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		{
			//Do the visual gate here
			stateManager.gateVisual = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(stateManager.gateVisual, LoadModel("LV8GateGeo.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(stateManager.gateVisual);
		}
		{
			//Add static hazards here: ICE
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV8Ice.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, iceHazardGlowRGB);
			AddStaticHazard(hazard, HAZARD_ICE);
		}
		{
			//Add static hazards here
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV8Acid.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, acidGlowRGB);
			AddStaticHazard(hazard, HAZARD_ACID);
		}
		SetDirectionLight(0.6f, 0.6f, 0.61f, -1.6f, -3.0f, 1.0f);
		//Set glow components.
		torchGlow = registry.AddComponent<GlowComponent>(torch, 0.6f, 0.9f, 0.8f);
		stageGlow = registry.AddComponent<GlowComponent>(stage, iceStageGlowRGB);	// Ice glow.
		portalGlow = registry.AddComponent<GlowComponent>(stateManager.gateVisual, portalGlowRGB);	// Portal glow
		break;
	case 9:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV9Geo.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV9Hitbox.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV9Nav.mdl"));
		torchModel = registry.AddComponent<ModelBonelessComponent>(torch, LoadModel("LV9Torch.mdl"));
		AddStaticHazard(stateManager.naviagtion, HAZARD_NAV);
		SetDirectionLight(0.55f, 0.55f, 0.60f, -1.6f, -3.0f, 1.0f);
		//Set glow components.
		torchGlow = registry.AddComponent<GlowComponent>(torch, 0.6f, 0.9f, 0.8f);
		stageGlow = registry.AddComponent<GlowComponent>(stage, iceStageGlowRGB);	// Ice glow.
		{
			//Add static hazards here: ICE
			EntityID hazard = registry.CreateEntity();
			registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LV9Ice.mdl"));
			TransformComponent* transform = registry.AddComponent<TransformComponent>(hazard);
			registry.AddComponent<GlowComponent>(hazard, iceHazardGlowRGB);
			AddStaticHazard(hazard, HAZARD_ICE);
		}
		break;
	default:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
		ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
		phc->Load("default");
		break;
	}
	

	stageModel->shared.colorMultiplicativeRed = stageVars.rm;
	stageModel->shared.colorMultiplicativeGreen = stageVars.gm;
	stageModel->shared.colorMultiplicativeBlue = stageVars.bm;
	stageModel->shared.colorAdditiveRed = stageVars.ra;
	stageModel->shared.colorAdditiveGreen = stageVars.ga;
	stageModel->shared.colorAdditiveBlue = stageVars.ba;

	/*registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());*/

	// Stage (Default)
	TransformComponent* transform = registry.AddComponent<TransformComponent>(stage);
	transform->scaleX = stageVars.scaleX;
	transform->scaleY = stageVars.scaleY;
	transform->scaleZ = stageVars.scaleZ;
	transform->positionX = stageVars.offsetX;
	transform->positionY = stageVars.offsetY;
	transform->positionZ = stageVars.offsetZ;
	TransformComponent* transformG = registry.AddComponent<TransformComponent>(gate, transform);
	TransformComponent* transformH = registry.AddComponent<TransformComponent>(hitbox, transform);
	TransformComponent* transformN = registry.AddComponent<TransformComponent>(stateManager.naviagtion, transform);
	TransformComponent* transformT = registry.AddComponent<TransformComponent>(torch, transform);
	RenderGeometryIndependentCollision(stage, gate, hitbox);
	
	stateManager.gate = gate;
	
#ifndef _DEBUG
	registry.DestroyEntity(hitbox);
	registry.DestroyEntity(stateManager.naviagtion);
#else
	if (visualizeStage == true)
	{
		stateManager.hitboxVis = hitbox;
		transformN->facingZ = -1.0f;
	}
	else
	{
		registry.DestroyEntity(hitbox);
		registry.DestroyEntity(stateManager.naviagtion);
	}
	
	
#endif // _DEBUG

	return stage;
}

EntityID SetUpHazard(const StaticHazardType& type, const float scale, const float ra, const float ga, const float ba, const float rm, const float gm, const float bm, const float gamma)
{
	EntityID hazard;// = registry.CreateEntity();

	bool succeded = false;
	while (!succeded)
	{
		int scalePosMod = 5 * (int)scale;
		float randX = scalePosMod + (float)(rand() % (100 - 2 * scalePosMod)) - 50.0f;
		float randZ = scalePosMod + (float)(rand() % (100 - 2 * scalePosMod)) - 50.0f;
		if (randX * randX + randZ * randZ > 80)
		{
			float randScaleX = 5.0f + (float)((rand() % 100) * 0.1f);
			float randScaleZ = 5.0f + (float)((rand() % 100) * 0.1f);
			/*EntityID*/ hazard = registry.CreateEntity();
			ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
			hazardModel->shared.colorAdditiveRed = ra;
			hazardModel->shared.colorAdditiveGreen = ga;
			hazardModel->shared.colorAdditiveBlue = ba;
			hazardModel->shared.colorMultiplicativeRed = rm;
			hazardModel->shared.colorMultiplicativeGreen = gm;
			hazardModel->shared.colorMultiplicativeBlue = bm;
			hazardModel->shared.gammaCorrection = gamma;
			hazardModel->shared.castShadow = false;
			TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
			hazardTransform->positionX = randX;
			hazardTransform->positionY = 0.6f - ((float)type * .1f);
			hazardTransform->positionZ = randZ;
			hazardTransform->scaleX = randScaleX * scale;
			hazardTransform->scaleY = 1.f;
			hazardTransform->scaleZ = randScaleZ * scale;
			hazardTransform->facingX = cosf((float)rand());
			hazardTransform->facingZ = sinf((float)rand());
			AddStaticHazard(hazard, type);

			succeded = true;
		}
	}

	return hazard;
}

void SetupEnemyNavigationHelper(bool level8)
{
	EntityID entity = registry.CreateEntity();
	TransformComponent transform;
	transform.positionX = 0.f;
	if(level8)
		transform.positionY = 1.6f;
	else
		transform.positionY = 0.3f;
	transform.positionZ = 0.f;
	transform.mass = 1.f;
	transform.facingX = 0.f; transform.facingY = 0.f; transform.facingZ = 0.f;
	transform.scaleX = 0.420f; transform.scaleY = 0.420f; transform.scaleZ = 0.420f;
	

	registry.AddComponent<TransformComponent>(entity, transform);

	ModelBonelessComponent* model = nullptr;
	model = registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("FixedArrow.mdl"));
	if (model != nullptr)
	{
		//model->shared.gammaCorrection = 1.5f;
		model->shared.hasOutline = true;
	}
	registry.AddComponent<NavigationTrashComponentYouMustAccept>(entity);
}

EntityID SetupEnemy(EnemyType eType, float positionX , float positionY , float positionZ , int soulWorth, float mass ,
	float health , float moveSpeed , float damage, float attackSpeed ,  float scaleX, float scaleY, float scaleZ, float facingX ,
	float facingY , float facingZ, bool zacIndex0, bool zacIndex1, bool zacIndex2, bool zacIndex3, bool zacIndex4, bool worthLess)
{
	EntityID entity = registry.CreateEntity();
	TransformComponent transform;
	auto player = registry.GetComponent<PlayerComponent>(stateManager.player);
	

	assert(mass > 0.0f);
	if (mass == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			mass = 50.f;
		}
		else if (eType == EnemyType::hellhound || eType == EnemyType::empoweredHellhound)
		{
			mass = 90.f;
		}
		else if (eType == EnemyType::skeleton || eType == EnemyType::empoweredSkeleton)
		{
			mass = 70.f;
		}
		else if (eType == EnemyType::imp || eType == EnemyType::empoweredImp)
		{
			mass = 40.f;
		}
		else if (eType == EnemyType::minotaur)
		{
			mass = 300.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			mass = 666.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			mass = 666.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			mass = 666.f;
		}
	}
	if (health == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			health = 61.f;//60.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			health = 35.f;//150.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			health = 25.f; //100.f;
		}
		else if (eType == EnemyType::imp)
		{
			health = 18.f;
		}
		else if (eType == EnemyType::minotaur)
		{
			health = 171.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			health = 0;//200.f;
			float partHealth = 40.f; // this times 5 is the full starting strength
			if (zacIndex0)
				health += partHealth;
			if (zacIndex1)
				health += partHealth;
			if (zacIndex2)
				health += partHealth;
			if (zacIndex3)
				health += partHealth;
			if (zacIndex4)
				health += partHealth;
		

		}
		else if (eType == EnemyType::lucifer)
		{
			health = 1000.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			health = 1.f;
		}
		else if (eType == EnemyType::empoweredSkeleton)
		{
			health = 101.f;
		}
		else if (eType == EnemyType::empoweredHellhound)
		{
			health = 131.f;
		}
		else if (eType == EnemyType::empoweredImp)
		{
			health = 61.f; //same as eye fuck it
		}
	}
	if (moveSpeed == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			moveSpeed = 30.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			moveSpeed = 15.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			moveSpeed = 10.f;
		}
		else if (eType == EnemyType::imp || eType == EnemyType::empoweredImp)
		{
			moveSpeed = 3.f;
		}
		else if (eType == EnemyType::minotaur)
		{
			moveSpeed = 12.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			moveSpeed = 20.f; //starting speed
			float partSpeed = 2.5f; // each alive part makes it this much slower
			if (zacIndex0)
				moveSpeed -= partSpeed;
			if (zacIndex1)
				moveSpeed -= partSpeed;
			if (zacIndex2)
				moveSpeed -= partSpeed;
			if (zacIndex3)
				moveSpeed -= partSpeed;
			if (zacIndex4)
				moveSpeed -= partSpeed;
		}
		else if (eType == EnemyType::lucifer)
		{
			moveSpeed = 30.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			moveSpeed = 0.1f;
		}
		else if (eType == EnemyType::empoweredHellhound)
		{
			moveSpeed = 22.f; // :)
		}
		else if (eType == EnemyType::empoweredSkeleton)
		{
			moveSpeed = 22.f;
		}

	}
	if (damage == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			damage = 16.f; //Used to be 31
		}
		else if (eType == EnemyType::hellhound)
		{
			damage = 13.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			damage = 8.f;
		}
		else if (eType == EnemyType::imp)
		{
			damage = 15.f;
		}
		else if (eType == EnemyType::minotaur)
		{
			damage = 25.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			damage = 20.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			damage = 40.f; //66.6f????
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			damage = 0.f;
		}
		else if (eType == EnemyType::empoweredHellhound)
		{
			damage = 26.f;
		}
		else if (eType == EnemyType::empoweredSkeleton)
		{
			damage = 17.f;
		}
		else if (eType == EnemyType::empoweredImp)
		{
			damage = 24.f;
		}
	}
	if (attackSpeed == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			attackSpeed = 0.5f;
		}
		else if (eType == EnemyType::hellhound || eType == EnemyType::empoweredHellhound)
		{
			attackSpeed = 0.1f;
		}
		else if (eType == EnemyType::skeleton)
		{
			//NICLAS WAS HERE
			attackSpeed = 1.0f;
			//attackSpeed = 0.5f;
		}
		else if (eType == EnemyType::empoweredSkeleton)
		{
			//NICLAS WAS HERE
			attackSpeed = 2.0f;
		}
		else if (eType == EnemyType::imp)
		{
			attackSpeed = 0.8f;
		}
		else if (eType == EnemyType::empoweredImp)
		{
			attackSpeed = 1.0f;
		}
		else if (eType == EnemyType::minotaur)
		{
			attackSpeed = 3.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			attackSpeed = 0.25f;
			float partSpeed = 0.1f; // each alive part makes it this much slower
			if (zacIndex0)
				attackSpeed += partSpeed;
			if (zacIndex1)
				attackSpeed += partSpeed;
			if (zacIndex2)
				attackSpeed += partSpeed;
			if (zacIndex3)
				attackSpeed += partSpeed;
			if (zacIndex4)
				attackSpeed += partSpeed;
		}
		else if (eType == EnemyType::lucifer)
		{
			attackSpeed = 1.2f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			attackSpeed = 100.f;
		}
	}
	if (soulWorth == 6969.f)
	{
		if (eType == EnemyType::skeleton || eType == EnemyType::empoweredSkeleton)
		{
			soulWorth = 1;
		}
		else if (eType == EnemyType::hellhound || eType == EnemyType::empoweredHellhound)
		{
			soulWorth = 1;
		}
		else if (eType == EnemyType::eye)
		{
			soulWorth = 1;
		}
		else if (eType == EnemyType::imp || eType == EnemyType::empoweredImp)
		{
			soulWorth = 1;
		}
		else if (eType == EnemyType::minotaur)
		{
			soulWorth = 1;
		}
		else if (eType == EnemyType::tempBoss)
		{
			soulWorth = 0;
		}
		else if (eType == EnemyType::lucifer)
		{
			soulWorth = 0; //No Damned Soul(TM)s
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			soulWorth = 0;
		}
	}
	if (eType == EnemyType::tempBoss)
	{
		scaleX = 4;
		scaleY = 4;
		scaleZ = 4;
	}
	else if (eType == EnemyType::hellhound || eType == EnemyType::frozenHellhound || eType == EnemyType::empoweredHellhound)
	{
		scaleX = 1.f;
		scaleY = 1.f;
		scaleZ = 1.f;
	}
	else if (eType == EnemyType::lucifer)
	{
		scaleX = 3.f;
		scaleY = 3.f;
		scaleZ = 3.f;
	}

	transform.mass = mass;
	transform.facingX = facingX; transform.facingY = facingY; transform.facingZ = facingZ;

	transform.positionX = positionX; transform.positionY = positionY; transform.positionZ = positionZ;
	transform.scaleX = scaleX; transform.scaleY = scaleY; transform.scaleZ = scaleZ;
	registry.AddComponent<TransformComponent>(entity, transform);

	StatComponent* stat = registry.AddComponent<StatComponent>(entity, health, moveSpeed, damage, attackSpeed);
	registry.AddComponent<EnemyComponent>(entity, soulWorth, eType);

#ifdef DEBUG_HP
	// UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(entity);
	UIGameHealthComponent* uiHealth = registry.AddComponent<UIGameHealthComponent>(entity);
	uiElement->Setup("EmptyHealth", "", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("FullHealth", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
#endif

	//Model

	ModelSkeletonComponent* model = nullptr;
	if (eType == EnemyType::skeleton)
	{
		//registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("Skeleton.mdl"));
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Skeleton.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<SkeletonBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.0f, EnemyType::skeleton);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(SKELETON);
		if (player)
		{
			player->killThreshold++;
		}

		//Orange glow
		registry.AddComponent<GlowComponent>(entity, .95f, .5f, .0f);
	}
	else if (eType == EnemyType::hellhound)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->lavaAccelFactor = 1.0f;
		stat->lavaAnimFactor = 1.0f;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Hellhound.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<HellhoundBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 2.5f, EnemyType::hellhound);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(HELLHOUND);
		if (player)
		{
			player->killThreshold++;
		}

		registry.AddComponent<GlowComponent>(entity, .95f, .5f, .0f);
	}
	else if (eType == EnemyType::eye)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->baseCanWalkOnCrack = true;
		stat->canWalkOnCrack = true;
		stat->lavaAccelFactor = 1.0f;
		stat->lavaAnimFactor = 1.0f;
		stat->acidAccelFactor = 1.0f;
		stat->acidAnimFactor = 1.0f;
		stat->iceAccelFactor = 1.0f;
		stat->iceAnimFactor = 1.0f;


		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Eye.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<EyeBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 2.0f, EnemyType::eye, false);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(EYE);
		if (player)
		{
			player->killThreshold++;
		}

		registry.AddComponent<GlowComponent>(entity, 0.7f, 0.7f, 0.5f);	//Erika was here
	}
	else if (eType == EnemyType::imp)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->baseCanWalkOnCrack = true;
		stat->canWalkOnCrack = true;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Imp.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<ImpBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.f, EnemyType::imp, false);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(IMP);

		//// REMOVE ONCE WE HAVE THE IMP MODEL
		//model->shared.colorMultiplicativeRed = 0.2f;
		//model->shared.colorMultiplicativeBlue = 0.2f;
		//model->shared.colorMultiplicativeGreen = 0.2f;
		//model->shared.colorAdditiveRed = 0.8f;
		//model->shared.colorAdditiveBlue = 0.4f;
		//model->shared.colorAdditiveGreen = 0.8f;

		//model->shared.baseColorMultiplicativeRed = 0.2f;
		//model->shared.baseColorMultiplicativeBlue = 0.2f;
		//model->shared.baseColorMultiplicativeGreen = 0.2f;
		//model->shared.baseColorAdditiveRed = 0.8f;
		//model->shared.baseColorAdditiveBlue = 0.4f;
		//model->shared.baseColorAdditiveGreen = 0.8f;


		if (player)
		{
			player->killThreshold += 1;
		}
		registry.AddComponent<GlowComponent>(entity, 0.8f, 0.6f, 0.9f);	//Erika was here
	}
	else if (eType == EnemyType::minotaur)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->lavaAccelFactor = 1.0f;
		stat->lavaAnimFactor = 1.0f;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Minotaur.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<MinotaurBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.3f, EnemyType::minotaur);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(MINOTAUR);
		if (player)
		{
			player->killThreshold++;
		}
		registry.AddComponent<GlowComponent>(entity, 0.9f, 0.7f, 0);
	}
	else if (eType == EnemyType::tempBoss)
	{
		stat->hazardModifier = 0.0f;
		ModelSkeletonComponent* mod = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Splitboss.mdl"));
		mod->shared.hasOutline = true;
		registry.AddComponent<AnimationComponent>(entity);
		mod->shared.gammaCorrection = 1.5f;
		registry.AddComponent<TempBossBehaviour>(entity, 0, 0);
		TempBossBehaviour* tempBossComponent = registry.GetComponent<TempBossBehaviour>(entity);
		tempBossComponent->worthLess = worthLess;
		
		tempBossComponent->parts[0] = zacIndex0; // this is needed, DO NOT TOUCH
		tempBossComponent->parts[1] = zacIndex1;
		tempBossComponent->parts[2] = zacIndex2;
		tempBossComponent->parts[3] = zacIndex3;
		tempBossComponent->parts[4] = zacIndex4;

		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(MINIBOSS);

 		SetupEnemyCollisionBox(entity, 0.4f * scaleX, EnemyType::tempBoss);
		if (player)
		{
			player->killThreshold+=5;
		}
		//Orange glow
		registry.AddComponent<GlowComponent>(entity, .7f, .3f, 0.f);
	}
	else if (eType == EnemyType::lucifer)
	{
		stat->hazardModifier = 0.0f;
		ModelSkeletonComponent* mod = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("BossTest.mdl"));
		mod->shared.gammaCorrection = 1.5f;
		AnimationComponent* anim = registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<LuciferBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.2f * scaleX, EnemyType::lucifer);
		if (player)
		{
			player->killThreshold++;
		}

		//Sounds (Added by Joaquin)
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(BOSS);
		registry.AddComponent<GlowComponent>(entity, .1f, .2f, .6f);
	}
	else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
	{
		EntityID particlesVFX = registry.CreateEntity();	//no,  no,    size , offset xyz
		registry.AddComponent<GlowComponent>(entity, .0f, .05f, .15f);

		registry.AddComponent<ParticleComponent>(particlesVFX, 100.0f, 100.0f, 25.0f, 0.0f, 0.0f, -4.5f, 1, "\\AcidGround.mdl", VFX_PATTERN::SPAWN_BOSS);
		TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(particlesVFX);
		hazardTransform->positionX = transform.positionX;
		hazardTransform->positionY = 0.2f;
		hazardTransform->positionZ = transform.positionZ;
		hazardTransform->scaleX = 1.f;
		hazardTransform->scaleY = 1.0f;
		hazardTransform->scaleZ = 1.f;
		hazardTransform->facingX = 0.0000f;
		hazardTransform->facingZ = 0.0000f;
		hazardTransform->facingY = 0.00001f;

		AddTimedEventComponentStart(particlesVFX, 4.f, BossSpawnwaveEnd);

		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		if (eType == EnemyType::frozenHellhound)
		{
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Hellhound.mdl"));
			//Sounds (Added by Joaquin)
			SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
			scp->Load(HELLHOUND);
		}
		else if (eType == EnemyType::frozenImp)
		{
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Imp.mdl"));
			//Sounds (Added by Joaquin)
			SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
			scp->Load(IMP);
		}
		if (eType == EnemyType::frozenEye)
		{
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Eye.mdl"));
			//Sounds (Added by Joaquin)
			SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
			scp->Load(EYE);
		}
		model->shared.colorMultiplicativeRed = 0.4f;
		model->shared.colorMultiplicativeBlue = 0.4f;
		model->shared.colorMultiplicativeGreen = 0.4f;
		model->shared.colorAdditiveRed = 0.4f;
		model->shared.colorAdditiveBlue = 0.8f;
		model->shared.colorAdditiveGreen = 0.4f;

		model->shared.baseColorMultiplicativeRed = 0.4f;
		model->shared.baseColorMultiplicativeBlue = 0.4f;
		model->shared.baseColorMultiplicativeGreen = 0.4f;
		model->shared.baseColorAdditiveRed = 0.4f;
		model->shared.baseColorAdditiveGreen = 0.4f;
		model->shared.baseColorAdditiveBlue = 0.8f;
		model->shared.hasOutline = true;

		registry.AddComponent<AnimationComponent>(entity);
		FrozenBehaviour* behev = registry.AddComponent<FrozenBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.5f, EnemyType::frozenHellhound);
		if (eType == EnemyType::frozenHellhound)
		{
			behev->type = EnemyType::frozenHellhound;
		}
		else if (eType == EnemyType::frozenImp)
		{
			behev->type = EnemyType::frozenImp; 
		}
		if (eType == EnemyType::frozenEye)
		{
			behev->type = EnemyType::frozenEye;
		}
	}
	else if (eType == EnemyType::empoweredHellhound)
	{
		//Make them immune to ice stuff, but how
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->lavaAccelFactor = 1.0f;
		stat->lavaAnimFactor = 1.0f;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Hellhound.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		HellhoundBehaviour* hhb = registry.AddComponent<HellhoundBehaviour>(entity);
		hhb->isEmpoweredDoggo = true;
		SetupEnemyCollisionBox(entity, 1.5f, EnemyType::hellhound);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(HELLHOUND);
		if (player)
		{
			player->killThreshold++;
		}

		//Hue shift for strongdogs (Darker mult on the red parts, then add blue?)
		model->shared.baseColorMultiplicativeRed = 0.3f;
		//model->shared.baseColorMultiplicativeRed = 0.4f;
		model->shared.baseColorMultiplicativeGreen = 0.65f;
		//model->shared.baseColorMultiplicativeBlue = 0.7f;
		//model->shared.baseColorAdditiveBlue = 0.2f;

		//Gotta do the color, not just base, not sure why
		model->shared.colorMultiplicativeRed = 0.3f;
		model->shared.colorMultiplicativeGreen = 0.65f;
		//model->shared.colorMultiplicativeBlue = 0.7f;
		//model->shared.colorAdditiveBlue = 0.2f;

		registry.AddComponent<GlowComponent>(entity, .0f, .75f, .95f);

	}
	else if (eType == EnemyType::empoweredSkeleton)
	{
		//registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("Skeleton.mdl"));
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Skeleton.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<SkeletonBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.0f, EnemyType::skeleton);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(SKELETON);
		if (player)
		{
			player->killThreshold++;
		}

		//Reduce the red hue of the base color for skelington (reduce green a little bit to make up for the imbalance)
		model->shared.baseColorMultiplicativeRed = 0.6f;
		model->shared.baseColorMultiplicativeGreen = 0.8f;
		//model->shared.colorAdditiveBlue = 0.2f;

		model->shared.colorMultiplicativeRed = 0.6f;
		model->shared.colorMultiplicativeGreen = 0.8f;
		//model->shared.baseColorAdditiveBlue = 0.2f;

		//Blue glow instead of orange
		registry.AddComponent<GlowComponent>(entity, .0f, .75f, .95f);
	}
	else if (eType == EnemyType::empoweredImp)
	{
		//No need to touch any hazard stuff, sweet
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->baseCanWalkOnCrack = true;
		stat->canWalkOnCrack = true;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Imp.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<ImpBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.f, EnemyType::imp, false);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(IMP);

		//Same as with doggo
		model->shared.colorMultiplicativeRed = 0.3f;
		model->shared.colorAdditiveBlue = 0.2f;

		model->shared.baseColorMultiplicativeRed = 0.3f;
		model->shared.baseColorAdditiveBlue = 0.2f;

		if (player)
		{
			player->killThreshold += 1;
		}
		registry.AddComponent<GlowComponent>(entity, .5f, .0f, .65f);
	}


	if (model != nullptr)
	{
		model->shared.gammaCorrection = 1.5f;
		model->shared.hasOutline = true;
	}
	CreatePointLight(entity, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);
	return entity;
}

void CreatePlayer(float positionX, float positionY, float positionZ, float mass, float health, float moveSpeed, float damage, float attackSpeed, int soulWorth, float facingX, float facingY, float facingZ, float scaleX, float scaleY, float scaleZ)
{
	//Create player
	stateManager.player = registry.CreateEntity(ENT_PERSIST_LEVEL);

	ModelSkeletonComponent* model = registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerLP.mdl"));
	model->shared.colorMultiplicativeRed = 1.25f;
	model->shared.colorMultiplicativeGreen = 1.25f;
	model->shared.colorMultiplicativeBlue = 1.25f;
	model->shared.gammaCorrection = 1.5f;
	model->shared.hasOutline = true;
	BlendAnimationComponent* animation = registry.AddComponent<BlendAnimationComponent>(stateManager.player, BlendAnimationComponent());
	animation->lower.aAnim = ANIMATION_IDLE;
	animation->lower.aAnimTime = 0.5f;
	animation->lower.aAnimIdx = 0;
	animation->lower.aAnimTimeFactor = 1.0f;

	animation->upper.aAnim = ANIMATION_IDLE;
	animation->upper.aAnimTime = 0.5f;
	animation->upper.aAnimIdx = 0;
	animation->upper.aAnimTimeFactor = 1.0f;
	//stateManager.player Sounds
	LoadPlayerSounds();

	// Player (Default)
	TransformComponent* playerTransform = registry.AddComponent<TransformComponent>(stateManager.player);
	playerTransform->facingZ = facingZ;
	playerTransform->mass = mass;
	playerTransform->positionX = positionX;
	playerTransform->positionY = positionY;
	playerTransform->positionZ = positionZ;
	playerTransform->scaleX = scaleX; //We never actually set these before :)
	playerTransform->scaleY = scaleY;
	playerTransform->scaleZ = scaleZ;

	registry.AddComponent<StatComponent>(stateManager.player,health, moveSpeed, damage, attackSpeed); //Hp, MoveSpeed, Damage, AttackSpeed
	PlayerComponent* pc = registry.AddComponent<PlayerComponent>(stateManager.player);
	pc->UpdateSouls(soulWorth);

	registry.AddComponent<ControllerComponent>(stateManager.player);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	registry.AddComponent<PointOfInterestComponent>(stateManager.player, poic);
	SetupPlayerCollisionBox(stateManager.player, 1.0f);
	MouseComponentAddComponent(stateManager.player);
	CreatePointLight(stateManager.player, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);

	// ## ALEX CODE ##
	//ParticleComponent* pSlashComp = registry.AddComponent<ParticleComponent>(stateManager.player, 5.0f, 50.0f, 5.5f, 0.0f, 10.0f, 0.0f, 1, "\\SwordSlash.mdl", VFX_PATTERN::SWORD);
	// ## EO ALEX CODE ##



	// UI
	SetUpAdvancedHealthBar(stateManager.player);
	//UIComponent* uiElement = registry.AddComponent<UIComponent>(stateManager.player);
	//
	////Setup + Health
	//uiElement->Setup("EmptyHealth", "", DSFLOAT2(-0.8f, 0.8f));
	//uiElement->AddImage("FullHealth", DSFLOAT2(-0.8f, 0.8f));
	//UIGameHealthComponent* uiHealth = registry.AddComponent<UIGameHealthComponent>(stateManager.player);

	////Souls
	//uiElement->AddImage("EmptyHealth", DSFLOAT2(-0.8f, 0.6f));
	//uiElement->AddText(" ",uiElement->m_Images[0].baseUI.GetOriginalBounds(), DSFLOAT2(-0.8f, 0.6f));
	//UIPlayerSoulsComponent* uiSouls = registry.AddComponent<UIPlayerSoulsComponent>(stateManager.player);
	
	////Relics
	//uiElement->AddImage("TempRelicHolder11", DSFLOAT2(-0.95f, -0.1f));
	//UIPlayerRelicsComponent* uiRelics = registry.AddComponent<UIPlayerRelicsComponent>(stateManager.player);
	//OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stateManager.player);

	//GlowComponent* player_glow = registry.AddComponent<GlowComponent>(stateManager.player, 1, 0, 0);

	// Create weapon
	stateManager.weapon = registry.CreateEntity(ENT_PERSIST_LEVEL);

	ModelSkeletonComponent* weapon_model = registry.AddComponent<ModelSkeletonComponent>(stateManager.weapon, LoadModel("PlayerBoneWeapon.mdl"));
	weapon_model->shared.colorMultiplicativeRed = 1.25f;
	weapon_model->shared.colorMultiplicativeGreen = 1.25f;
	weapon_model->shared.colorMultiplicativeBlue = 1.25f;
	weapon_model->shared.gammaCorrection = 1.5f;
	weapon_model->shared.hasOutline = true;

	AnimationComponent* weapon_animation = registry.AddComponent<AnimationComponent>(stateManager.weapon, AnimationComponent());
	weapon_animation->aAnim = ANIMATION_IDLE;
	weapon_animation->aAnimTime = 0.5f;
	weapon_animation->aAnimIdx = 0;
	weapon_animation->aAnimTimeFactor = 1.0f;

	TransformComponent* weapon_transform = registry.AddComponent<TransformComponent>(stateManager.weapon);
	weapon_transform->facingZ = facingZ;
	weapon_transform->mass = mass;

	FollowerComponent* weapon_follow = registry.AddComponent<FollowerComponent>(stateManager.weapon, stateManager.player);

	GlowComponent* weaponGlow = registry.AddComponent<GlowComponent>(stateManager.weapon, 0, 0, 0/*0.8, 0.7*/);
}

void SetPlayerPosition(float positionX, float positionY, float positionZ)
{
	TransformComponent* playerTransform = registry.AddComponent<TransformComponent>(stateManager.player);
	playerTransform->positionX = positionX;
	playerTransform->positionY = positionY;
	playerTransform->positionZ = positionZ;
}

void LoadPlayerSounds()
{
	SoundComponent* scp = registry.AddComponent<SoundComponent>(stateManager.player);
	scp->Load(PLAYER);
}

void ReloadPlayerNonGlobals()

{
	if (registry.GetComponent<PlayerComponent>(stateManager.player) == nullptr)
		return; //Ain't no way we doing this function if player doesn't exist yet

	ModelSkeletonComponent* modelLoaded = registry.GetComponent<ModelSkeletonComponent>(stateManager.player);
	if (modelLoaded == nullptr)
	{
		modelLoaded= registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerLP.mdl"));
		modelLoaded->shared.colorMultiplicativeRed = 1.25f;
		modelLoaded->shared.colorMultiplicativeGreen = 1.25f;
		modelLoaded->shared.colorMultiplicativeBlue = 1.25f;
		modelLoaded->shared.gammaCorrection = 1.5f;
	}
	BlendAnimationComponent* animationLoaded = registry.GetComponent<BlendAnimationComponent>(stateManager.player);
	if (animationLoaded == nullptr)
	{
		animationLoaded = registry.AddComponent<BlendAnimationComponent>(stateManager.player, BlendAnimationComponent());
		
	}
	animationLoaded->lower.aAnimTimeFactor = 1.0f;
	animationLoaded->upper.aAnimTimeFactor = 1.0f;

	// Player (Default)
	TransformComponent* playerTransform = registry.GetComponent<TransformComponent>(stateManager.player);
	if (playerTransform == nullptr)
	{
		playerTransform = registry.AddComponent<TransformComponent>(stateManager.player);
		playerTransform->mass = 80.0f; //We set this to 80 normally, but here this was 3, oof
		playerTransform->scaleX = playerTransform->scaleY = playerTransform->scaleZ = 1.f;
	}
	playerTransform->currentSpeedX = 0.0f;
	playerTransform->currentSpeedZ = 0.0f;
	playerTransform->positionX = 0.0f;
	playerTransform->positionZ = 0.0f;
	playerTransform->positionY = 0.0f;

	ControllerComponent* controller = registry.GetComponent<ControllerComponent>(stateManager.player);
	if (controller == nullptr)
	{
		controller = registry.AddComponent<ControllerComponent>(stateManager.player);
	}
	controller->enabled = 1;
	PointOfInterestComponent* cameraPoint = registry.GetComponent<PointOfInterestComponent>(stateManager.player);
	if (cameraPoint == nullptr)
	{
		cameraPoint = registry.AddComponent<PointOfInterestComponent>(stateManager.player);
		cameraPoint->weight = 10.0f;
	}
	ReleaseTimedEvents(stateManager.player);
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(stateManager.player);
	if (hitbox)
	{
		registry.RemoveComponent<HitboxComponent>(stateManager.player);
	}
	SetupPlayerCollisionBox(stateManager.player, 1.0f);
	MouseComponentAddComponent(stateManager.player);

	int squashStretch1 = AddTimedEventComponentStart(stateManager.player, 0.0f, ResetSquashStretch);
	CreatePointLight(stateManager.player, 0.7f, 0.7f, 0.7f,  0.0f, 0.5f, 0.0f, 2.0f, 1.0f);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	player->isAttacking = false;
	player->isDashing = false;

	StatComponent* stats = registry.GetComponent<StatComponent>(stateManager.player);
	stats->SetSpeedMult(1.0f);
}

EntityID RandomPlayerEnemy(EnemyType enemyType) {
	int pixelValue = 0;
	GridPosition gridPos = {0};
	GeometryIndependentComponent* geoCo = nullptr;
	for (auto entity : View<GeometryIndependentComponent>(registry))
	{
		geoCo = registry.GetComponent<GeometryIndependentComponent>(entity);
	}
	do
	{
		//gridPos.x = (int)(((float)GI_TEXTURE_DIMENSIONS * 0.33f) + ((float)(rand() % GI_TEXTURE_DIMENSIONS) * 0.33f));
		//gridPos.z = (int)(((float)GI_TEXTURE_DIMENSIONS * 0.33f) + ((float)(rand() % GI_TEXTURE_DIMENSIONS) * 0.33f));
		gridPos.x = rand() % GI_TEXTURE_DIMENSIONS;
		gridPos.z = rand() % GI_TEXTURE_DIMENSIONS;
		
		pixelValue = giTexture->texture[gridPos.z][gridPos.x];
	} while (pixelValue != 1);

	
	Coordinate2D coords = GridOnPosition(gridPos, geoCo);
	EntityID enemy = SetupEnemy(enemyType, coords.x, 0.f, coords.z);
	SetHitboxIsPlayer(enemy, 1, true);
	registry.AddComponent<PlayerComponent>(enemy);
	StatComponent* stats = registry.GetComponent<StatComponent>(enemy);
	return enemy;
}

void SetScoreboardUI()
{
	EntityID scoreBoard = registry.CreateEntity(ENT_PERSIST_LEVEL);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(scoreBoard);
	uiElement->Setup("SettingsPanel", "", "Run Completed!", DSFLOAT2(0.0f, 0.0f), DSFLOAT2(1.5f, 1.0f), 35.0f);
	uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.6f));

	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(scoreBoard);
	OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(scoreBoard);

	uiElement->AddImage("ButtonMedium", DSFLOAT2(-0.2f, -0.5f), DSFLOAT2(1.0f, 1.0f), false);
	uiElement->AddText("New Run", uiElement->m_Images[0].baseUI.GetBounds(), DSFLOAT2(-0.2f, -0.5f));

	onClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::MainMenu::Start, UIFunctions::OnClick::None);
	onHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnHover::Image);

	uiElement->AddImage("ButtonMedium", DSFLOAT2(0.2f, -0.5f), DSFLOAT2(1.0f, 1.0f), false);
	uiElement->AddText("Main Menu", uiElement->m_Images[1].baseUI.GetBounds(), DSFLOAT2(0.2f, -0.5f));

	onClick->Add(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunctions::Game::SetMainMenu, UIFunctions::OnClick::None);
	onHover->Add(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunctions::OnHover::Image);

	DSFLOAT2 offsetPixelCoords = { abs(uiElement->m_BaseImage.baseUI.GetPixelCoords().x + 32.0f) ,
						   abs(uiElement->m_BaseImage.baseUI.GetPixelCoords().y + 32.0f) };

	DSFLOAT2 uiCoords = { (offsetPixelCoords.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
						-1 * ((offsetPixelCoords.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

	const int amount = 7;
	const char texts[amount][32] =
	{
		"Time: ",

		"Leftover Souls: ",
		"Spent Souls: ",
		"Total Souls: ",

		"Damage Done: ",

		"Damage Taken: ",

		"Healing Done: ",

	};

	for (int i = 0; i < amount; i++)
		uiElement->AddText(texts[i], DSBOUNDS(0.0f, 0.0f, 300.0f, 0.0f), DSFLOAT2(uiCoords.x + 0.6f, uiCoords.y - 0.4f - (0.1f * i)), DSFLOAT2(1.0f, 1.0f),
			20.0f ,DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	registry.AddComponent<UIGameScoreboardComponent>(scoreBoard);

	uiElement->SetAllVisability(false);
}

void UpdateScoreBoardUI(bool won)
{
	UIComponent* playerUI = registry.GetComponent<UIComponent>(stateManager.player);
	playerUI->SetAllVisability(false);

	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	StatComponent* playerStats = registry.GetComponent<StatComponent>(stateManager.player);

	for (auto entity : View<UIComponent, UIGameScoreboardComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UIGameScoreboardComponent* uiScore = registry.GetComponent<UIGameScoreboardComponent>(entity);

		if (!uiElement->m_BaseImage.baseUI.GetVisibility())
			uiElement->SetAllVisability(true);

		if (!won)
			uiElement->m_BaseText.m_Text = "Run Failed!";

		RedrawUI();
		gameSpeed = 0.0f;
		SetPaused(true);

		const int amount = 7;
		ML_String texts[amount] =
		{
			GetDigitalMinuteClock().c_str(),

			("Leftover Souls: " + std::to_string(player->GetSouls())).c_str(),
			("Spent Souls: " + std::to_string(player->GetTotalSouls() - player->GetSouls())).c_str(),
			("Total Souls: " + std::to_string(player->GetTotalSouls())).c_str(),
			("Damage Done: " + std::to_string((int)playerStats->GetDamageDone())).c_str(),
			("Healing Done: " + std::to_string((int)playerStats->GetHealthRecovered())).c_str(),
			("Damage Taken: " + std::to_string((int)playerStats->GetDamageTaken())).c_str(),

		};

		for (int i = 2; i < amount + 2; i++)
		{
			uiElement->m_Texts[i].SetText(texts[i - 2].c_str(), uiElement->m_BaseImage.baseUI.GetBounds(), 20.0f,
				DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}
	}
}

void SetupEnemyCounter()
{
	EntityID counterEntity = registry.CreateEntity(ENT_PERSIST_LEVEL);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(counterEntity);
	uiElement->Setup("ButtonSmallHoverBloody", "", "Enemies: 0", DSFLOAT2(0.8f, 0.8f));

	registry.AddComponent<UIGameEnemyCounterComponent>(counterEntity);
}

void SetupTimer()
{
	EntityID timeEntity = registry.CreateEntity(ENT_PERSIST_LEVEL);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(timeEntity);
	UIGameTimeComponent* timer = registry.AddComponent<UIGameTimeComponent>(timeEntity);

	uiElement->Setup("ButtonSmall", "", "Time: 0", DSFLOAT2(0.8f, 0.65f));

	if (!GetVisualTimer())
	{
		uiElement->m_BaseText.baseUI.SetVisibility(false);
		uiElement->m_BaseImage.baseUI.SetVisibility(false);
	}

}
