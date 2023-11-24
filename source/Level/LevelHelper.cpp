#include "Levels\LevelHelper.h"
#include "Registry.h"
#include "Components.h"
#include "Hitbox.h"
#include "Model.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "UIComponents.h"
#include "UIButtonFunctions.h"
#include "SDLHandler.h"


EntityID SetUpStage(StageSetupVariables& stageVars)
{
	EntityID stage = registry.CreateEntity();
	EntityID wall = registry.CreateEntity();
	EntityID hitbox = registry.CreateEntity();
	EntityID gate = registry.CreateEntity();
	EntityID noclip = registry.CreateEntity();//Decorations
	ModelBonelessComponent* stageModel;
	ModelBonelessComponent* wallModel;
	ModelBonelessComponent* hitboxModel;
	ModelBonelessComponent* gateModel;
	ModelBonelessComponent* noclipModel;
	switch (stageVars.stageNr)
	{
	case 0:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV1Floor.mdl"));
		wallModel = registry.AddComponent<ModelBonelessComponent>(wall, LoadModel("LV1Walls.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV1Gate.mdl"));
		noclipModel = registry.AddComponent<ModelBonelessComponent>(noclip, LoadModel("LV1Noclip.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV1Hitbox.mdl"));

		break;
	case 1:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV1Floor.mdl"));
		wallModel = registry.AddComponent<ModelBonelessComponent>(wall, LoadModel("LV1Walls.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV1Gate.mdl"));
		noclipModel = registry.AddComponent<ModelBonelessComponent>(noclip, LoadModel("LV1Noclip.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV1Hitbox.mdl"));
		break;
	case 2:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV2Floor.mdl"));
		wallModel = registry.AddComponent<ModelBonelessComponent>(wall, LoadModel("LV2Wall.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV2Gate.mdl"));
		noclipModel = registry.AddComponent<ModelBonelessComponent>(noclip, LoadModel("LV2Noclip.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV2Hitbox.mdl"));
		break;
	case 3:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV3Floor.mdl"));
		wallModel = registry.AddComponent<ModelBonelessComponent>(wall, LoadModel("LV3Walls.mdl"));
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV3Gate.mdl"));
		noclipModel = registry.AddComponent<ModelBonelessComponent>(noclip, LoadModel("LV3Noclip.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV3Hitbox.mdl"));
		break;
	case 4:
		stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("LV4Floor.mdl"));
		wallModel = nullptr;
		gateModel = registry.AddComponent<ModelBonelessComponent>(gate, LoadModel("LV4Gate.mdl"));
		noclipModel = registry.AddComponent<ModelBonelessComponent>(noclip, LoadModel("LV4Noclip.mdl"));
		hitboxModel = registry.AddComponent<ModelBonelessComponent>(hitbox, LoadModel("LV4Hitbox.mdl"));
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
	TransformComponent* transformW = registry.AddComponent<TransformComponent>(wall, transform);
	TransformComponent* transformG = registry.AddComponent<TransformComponent>(gate, transform);
	TransformComponent* transformN = registry.AddComponent<TransformComponent>(noclip, transform);
	TransformComponent* transformH = registry.AddComponent<TransformComponent>(hitbox, transform);
	RenderGeometryIndependentCollision(stage, wall, hitbox);

	
	
#ifndef _DEBUG
	registry.DestroyEntity(hitbox);
#else
	if (visualizeStage == true)
	{
		stateManager.hitboxVis = hitbox;
	}
	else
	{
		registry.DestroyEntity(hitbox);
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
			hazardModel->castShadow = false;
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

EntityID SetupEnemy(EnemyType eType, float positionX , float positionY , float positionZ , float mass ,
	float health , float moveSpeed , float damage, float attackSpeed , int soulWorth, float scaleX, float scaleY, float scaleZ, float facingX ,
	float facingY , float facingZ, bool zacIndex0, bool zacIndex1, bool zacIndex2, bool zacIndex3, bool zacIndex4)
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
			mass = 100.f;
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
			mass = 80.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			mass = 666.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			mass = 500.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			mass = 500.f;
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
			health = 120.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			health = 0;//400.f;
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
			health = 400.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			health = 1.f;
		}
		else if (eType == EnemyType::empoweredSkeleton)
		{
			health = 75.f;
		}
		else if (eType == EnemyType::empoweredHellhound)
		{
			health = 90.f;
		}
		else if (eType == EnemyType::empoweredImp)
		{
			health = 61.f; //same as eye fuck it
		}
	}
	else if (eType == EnemyType::tempBoss) // if we want a weaker version of the boss later in game, we can specify the health
	{
		
		float partHealth = health / 5.f; // this times 5 is the full starting strength
		health = 0;
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
			moveSpeed = 10.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			moveSpeed = 0.1f;
		}
		else if (eType == EnemyType::empoweredHellhound)
		{
			moveSpeed = 22.5f; // :)
		}
		else if (eType == EnemyType::empoweredSkeleton)
		{
			moveSpeed = 15.f;
		}

	}
	if (damage == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			damage = 31.f;
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
			damage = 13.f;
		}
		else if (eType == EnemyType::minotaur)
		{
			damage = 15.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			damage = 30.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			damage = 60.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			damage = 0.f;
		}
		else if (eType == EnemyType::empoweredHellhound)
		{
			damage = 24.f;
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
		else if (eType == EnemyType::skeleton || eType == EnemyType::empoweredSkeleton)
		{
			//NICLAS WAS HERE
			attackSpeed = 1.0f;
			//attackSpeed = 0.5f;
		}
		else if (eType == EnemyType::imp || eType == EnemyType::empoweredImp)
		{
			attackSpeed = 0.8f;
		}
		else if (eType == EnemyType::minotaur)
		{
			attackSpeed = 3.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			attackSpeed = 0.25f;
			float partSpeed = 0.05f; // each alive part makes it this much slower
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
			attackSpeed = 0.8f;
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
			soulWorth = 666;
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
	uiElement->Setup("ExMenu/EmptyHealth", "", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("ExMenu/FullHealth", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
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
		SetupEnemyCollisionBox(entity, 1.5f, EnemyType::hellhound);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(HELLHOUND);
		if (player)
		{
			player->killThreshold++;
		}
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
	}
	else if (eType == EnemyType::imp)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->baseCanWalkOnCrack = true;
		stat->canWalkOnCrack = true;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("EyePlaceholder.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<ImpBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.f, EnemyType::imp, false);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(IMP);

		// REMOVE ONCE WE HAVE THE IMP MODEL
		model->shared.colorMultiplicativeRed = 0.2f;
		model->shared.colorMultiplicativeBlue = 0.2f;
		model->shared.colorMultiplicativeGreen = 0.2f;
		model->shared.colorAdditiveRed = 0.8f;
		model->shared.colorAdditiveBlue = 0.4f;
		model->shared.colorAdditiveGreen = 0.8f;

		model->shared.baseColorMultiplicativeRed = 0.2f;
		model->shared.baseColorMultiplicativeBlue = 0.2f;
		model->shared.baseColorMultiplicativeGreen = 0.2f;
		model->shared.baseColorAdditiveRed = 0.8f;
		model->shared.baseColorAdditiveBlue = 0.4f;
		model->shared.baseColorAdditiveGreen = 0.8f;


		if (player)
		{
			player->killThreshold += 1;
		}
	}
	else if (eType == EnemyType::minotaur)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->lavaAccelFactor = 1.0f;
		stat->lavaAnimFactor = 1.0f;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("BossTest.mdl"));
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
	}
	else if (eType == EnemyType::tempBoss)
	{
		stat->hazardModifier = 0.0f;
		ModelSkeletonComponent* mod = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Skeleton.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		mod->shared.gammaCorrection = 1.5f;
		registry.AddComponent<TempBossBehaviour>(entity, 0, 0);
		TempBossBehaviour* tempBossComponent = registry.GetComponent<TempBossBehaviour>(entity);
		
		tempBossComponent->parts[0] = zacIndex0; // this is needed, DO NOT TOUCH
		tempBossComponent->parts[1] = zacIndex1;
		tempBossComponent->parts[2] = zacIndex2;
		tempBossComponent->parts[3] = zacIndex3;
		tempBossComponent->parts[4] = zacIndex4;

		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(SKELETON);

		SetupEnemyCollisionBox(entity, 0.4f * scaleX, EnemyType::tempBoss);
		if (player)
		{
			player->killThreshold+=5;
		}
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
	}
	else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		if (eType == EnemyType::frozenHellhound)
		{
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Hellhound.mdl"));
		}
		else if (eType == EnemyType::frozenImp)
		{
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("EyePlaceholder.mdl"));
		}
		if (eType == EnemyType::frozenEye)
		{
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Eye.mdl"));
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

		registry.AddComponent<AnimationComponent>(entity);
		FrozenBehaviour* behev = registry.AddComponent<FrozenBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.5f, EnemyType::frozenHellhound);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
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
		scp->Load(HELLHOUND);
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
		//model->shared.baseColorMultiplicativeRed = 0.3f;
		model->shared.baseColorMultiplicativeRed = 0.3f;
		model->shared.baseColorMultiplicativeGreen = 0.7f;
		model->shared.baseColorMultiplicativeBlue = 0.7f;
		model->shared.baseColorAdditiveBlue = 0.2f;

		//Gotta do the color, not just base, not sure why
		model->shared.colorMultiplicativeRed = 0.3f;
		model->shared.colorMultiplicativeGreen = 0.7f;
		model->shared.colorMultiplicativeBlue = 0.7f;
		model->shared.colorAdditiveBlue = 0.2f;

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
		model->shared.colorMultiplicativeRed = 0.6f;
		model->shared.colorMultiplicativeGreen = 0.8f;
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
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("EyePlaceholder.mdl"));
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
	}


	if (model != nullptr)
	{
		model->shared.gammaCorrection = 1.5f;
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
	AnimationComponent* animation = registry.AddComponent<AnimationComponent>(stateManager.player, AnimationComponent());
	animation->aAnim = ANIMATION_IDLE;
	animation->aAnimTime = 0.5f;
	animation->aAnimIdx = 0;
	animation->aAnimTimeFactor = 1.0f;
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
	registry.AddComponent<PlayerComponent>(stateManager.player);

	registry.AddComponent<ControllerComponent>(stateManager.player);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	registry.AddComponent<PointOfInterestComponent>(stateManager.player, poic);
	SetupPlayerCollisionBox(stateManager.player, 1.0f);
	MouseComponentAddComponent(stateManager.player);
	CreatePointLight(stateManager.player, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);

	// UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(stateManager.player);
	
	//Setup + Health
	uiElement->Setup("ExMenu/EmptyHealth", "", DSFLOAT2(-0.8f, 0.8f));
	uiElement->AddImage("ExMenu/FullHealth", DSFLOAT2(-0.8f, 0.8f));
	UIGameHealthComponent* uiHealth = registry.AddComponent<UIGameHealthComponent>(stateManager.player);

	//Souls
	uiElement->AddImage("ExMenu/EmptyHealth", DSFLOAT2(-0.8f, 0.6f));
	uiElement->AddText(" ",uiElement->m_Images[0].baseUI.GetOriginalBounds(), DSFLOAT2(-0.8f, 0.6f));
	UIPlayerSoulsComponent* uiSouls = registry.AddComponent<UIPlayerSoulsComponent>(stateManager.player);
	
	//Relics
	uiElement->AddImage("TempRelicHolder11", DSFLOAT2(-0.95f, -0.1f));
	UIPlayerRelicsComponent* uiRelics = registry.AddComponent<UIPlayerRelicsComponent>(stateManager.player);
	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stateManager.player);

	// Create weapon
	stateManager.weapon = registry.CreateEntity(ENT_PERSIST_LEVEL);

	ModelSkeletonComponent* weapon_model = registry.AddComponent<ModelSkeletonComponent>(stateManager.weapon, LoadModel("AxeV1.mdl"));
	weapon_model->shared.colorMultiplicativeRed = 1.25f;
	weapon_model->shared.colorMultiplicativeGreen = 1.25f;
	weapon_model->shared.colorMultiplicativeBlue = 1.25f;
	weapon_model->shared.gammaCorrection = 1.5f;

	AnimationComponent* weapon_animation = registry.AddComponent<AnimationComponent>(stateManager.weapon, AnimationComponent());
	weapon_animation->aAnim = ANIMATION_IDLE;
	weapon_animation->aAnimTime = 0.5f;
	weapon_animation->aAnimIdx = 0;
	weapon_animation->aAnimTimeFactor = 1.0f;

	TransformComponent* weapon_transform = registry.AddComponent<TransformComponent>(stateManager.weapon);
	weapon_transform->facingZ = facingZ;
	weapon_transform->mass = mass;

	FollowerComponent* weapon_follow = registry.AddComponent<FollowerComponent>(stateManager.weapon, stateManager.player);
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
	ModelSkeletonComponent* modelLoaded = registry.GetComponent<ModelSkeletonComponent>(stateManager.player);
	if (modelLoaded == nullptr)
	{
		modelLoaded= registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerLP.mdl"));
		modelLoaded->shared.colorMultiplicativeRed = 1.25f;
		modelLoaded->shared.colorMultiplicativeGreen = 1.25f;
		modelLoaded->shared.colorMultiplicativeBlue = 1.25f;
		modelLoaded->shared.gammaCorrection = 1.5f;
	}
	AnimationComponent* animationLoaded = registry.GetComponent<AnimationComponent>(stateManager.player);
	if (animationLoaded == nullptr)
	{
		animationLoaded = registry.AddComponent<AnimationComponent>(stateManager.player, AnimationComponent());
		
	}
	animationLoaded->aAnimTimeFactor = 1.0f;

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
	registry.GetComponent<TimedEventComponent>(stateManager.player);
	SetupPlayerCollisionBox(stateManager.player, 1.0f);
	MouseComponentAddComponent(stateManager.player);

	int squashStretch1 = AddTimedEventComponentStart(stateManager.player, 0.0f, ResetSquashStretch);
	CreatePointLight(stateManager.player, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	player->isAttacking = false;
	player->isDashing = false;
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
		gridPos.x = (int)(((float)GI_TEXTURE_DIMENSIONS * 0.33f) + ((float)(rand() % GI_TEXTURE_DIMENSIONS) * 0.33f));
		gridPos.z = (int)(((float)GI_TEXTURE_DIMENSIONS * 0.33f) + ((float)(rand() % GI_TEXTURE_DIMENSIONS) * 0.33f));
		
		pixelValue = giTexture->texture[gridPos.z][gridPos.x];
	} while (pixelValue != 1);

	
	Coordinate2D coords = GridOnPosition(gridPos, geoCo, false);
	EntityID enemy = SetupEnemy(enemyType, coords.x, 0.f, coords.z);
	SetHitboxIsPlayer(enemy, 1, true);
	registry.AddComponent<PlayerComponent>(enemy);
	StatComponent* stats = registry.GetComponent<StatComponent>(enemy);
	return enemy;
}

void SetScoreboardUI(EntityID stage)
{
	//Scoreboard UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(stage);
	uiElement->Setup("TempShopWindow3", "Run Completed!", DSFLOAT2(0.0f, 0.0f), DSFLOAT2(1.5f, 1.0f), 35.0f);
	uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.7f));

	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stage);
	OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(stage);

	uiElement->AddImage("ExMenu/ButtonBackground", DSFLOAT2(-0.2f, -0.6f), DSFLOAT2(0.5f, 0.6f));
	uiElement->AddText("\nNew Run", uiElement->m_Images[0].baseUI.GetBounds(), DSFLOAT2(-0.2f, -0.6f));

	onClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), 1, UIFunctions::MainMenu::Start);
	onHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnHover::Image);

	uiElement->AddImage("ExMenu/ButtonBackground", DSFLOAT2(0.2f, -0.6f), DSFLOAT2(0.5f, 0.6f));
	uiElement->AddText("\nMain Menu", uiElement->m_Images[1].baseUI.GetBounds(), DSFLOAT2(0.2f, -0.6f));

	onClick->Add(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), 1, UIFunctions::Game::SetMainMenu);
	onHover->Add(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunctions::OnHover::Image);

	DSFLOAT2 offsetUICoords = { abs(uiElement->m_BaseImage.baseUI.GetPixelCoords().x + 32.0f) ,
						   abs(uiElement->m_BaseImage.baseUI.GetPixelCoords().y + 32.0f) };

	DSFLOAT2 uiPixelCoords = { (offsetUICoords.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
						-1 * ((offsetUICoords.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

	DSFLOAT2 diffPos(uiPixelCoords.x + 1.1f, uiPixelCoords.y - 0.4f);

	uiElement->AddText("Difficulty", uiElement->m_Images[1].baseUI.GetBounds(), DSFLOAT2(diffPos.x, diffPos.y), DSFLOAT2(1.0f, 1.0f), 30.0f);
	uiElement->AddImage("Slider1", DSFLOAT2(diffPos.x, diffPos.y - 0.15f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("Slider2", DSFLOAT2(diffPos.x, diffPos.y - 0.25f), DSFLOAT2(1.0f, 1.0f));

	const int amount = 8;
	const char texts[amount][32] =
	{
		"Time: ", //index 3

		"Leftover Souls: ", //index 4
		"Spent Souls: ", //index 5
		"Total Souls: ", //index 6

		"Damage Done: (WIP)", //index 7

		//"Strongest Hit Dealt:",

		"Damage Taken: (WIP)", //index 8

		//"Strongest Hit Taken:",

		"Healing Done: (WIP)", //index 9

		//"Strongest Heal Done:"

		"Score: (WIP)" //index 10
	};

	for (int i = 0; i < amount; i++)
		uiElement->AddText(texts[i], DSBOUNDS(0.0f, 0.0f, 300.0f, 0.0f), DSFLOAT2(uiPixelCoords.x + 0.4f, uiPixelCoords.y - 0.3f - (0.1f * i)), DSFLOAT2(1.0f, 1.0f),
			20.0f ,DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	UIGameScoreboardComponent* scoreBoard = registry.AddComponent<UIGameScoreboardComponent>(stage);

	uiElement->SetAllVisability(false);
}