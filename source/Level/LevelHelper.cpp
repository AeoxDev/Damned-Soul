#include "Levels\LevelHelper.h"
#include "Registry.h"
#include "Components.h"
#include "Hitbox.h"
#include "Model.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "UIComponents.h"

EntityID SetUpStage(const float rm, const float gm, const float bm, const float ra, const float ga, const float ba, const float gamma)
{
	EntityID stage = registry.CreateEntity();

	ModelBonelessComponent* stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	stageModel->shared.colorMultiplicativeRed = rm;
	stageModel->shared.colorMultiplicativeGreen = gm;
	stageModel->shared.colorMultiplicativeBlue = bm;
	stageModel->shared.colorAdditiveRed = ra;
	stageModel->shared.colorAdditiveGreen = ga;
	stageModel->shared.colorAdditiveBlue = ba;

	/*registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());*/

	// Stage (Default)
	registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");

	return stage;
}

EntityID SetUpHazard(const StaticHazardType& type, const float scale, const float ra, const float ga, const float ba, const float rm, const float gm, const float bm, const float gamma)
{
	EntityID hazard;// = registry.CreateEntity();

	bool succeded = false;
	while (!succeded)
	{
		int scalePosMod = 5 * scale;
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
			hazardTransform->positionY = 0.6f - (type * .1f);
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
	float facingY , float facingZ, bool zacIndex1, bool zacIndex2, bool zacIndex3, bool zacIndex4, bool zacIndex5)
{
	EntityID entity = registry.CreateEntity();
	TransformComponent transform;
	auto player = registry.GetComponent<PlayerComponent>(stateManager.player);
	

	assert(mass > 0.0f);
	if (mass == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			mass = 35.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			mass = 100.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			mass = 70.f;
		}
		else if (eType == EnemyType::imp)
		{
			mass = 40.f;
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
			health = 15.f;//60.f;
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
			health = 20.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			health = 100;//400.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			health = 400.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			health = 1.f;
		}
	}
	if (moveSpeed == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			moveSpeed = 13.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			moveSpeed = 15.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			moveSpeed = 10.f;
		}
		else if (eType == EnemyType::imp)
		{
			moveSpeed = 3.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			moveSpeed = 10.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			moveSpeed = 10.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			moveSpeed = 0.1f;
		}
	}
	if (damage == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			damage = 12.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			damage = 12.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			damage = 8.f;
		}
		else if (eType == EnemyType::imp)
		{
			damage = 10.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			damage = 20.f;
		}
		else if (eType == EnemyType::lucifer)
		{
			damage = 15.f;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			damage = 0.f;
		}
	}
	if (attackSpeed == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			attackSpeed = 0.5f;
		}
		else if (eType == EnemyType::hellhound)
		{
			attackSpeed = 0.1f;
		}
		else if (eType == EnemyType::skeleton)
		{
			attackSpeed = 0.5f;
		}
		else if (eType == EnemyType::imp)
		{
			attackSpeed = 0.8f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			attackSpeed = 0.5f;
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
		if (eType == EnemyType::eye)
		{
			soulWorth = 2;
		}
		else if (eType == EnemyType::hellhound)
		{
			soulWorth = 3;
		}
		else if (eType == EnemyType::skeleton)
		{
			soulWorth = 1;
		}
		else if (eType == EnemyType::imp)
		{
			soulWorth = 3;
		}
		else if (eType == EnemyType::tempBoss)
		{
			soulWorth = 4;
		}
		else if (eType == EnemyType::lucifer)
		{
			soulWorth = 5;
		}
		else if (eType == EnemyType::frozenHellhound || eType == EnemyType::frozenEye || eType == EnemyType::frozenImp)
		{
			soulWorth = 0;
		}
	}
	if (eType == EnemyType::tempBoss)
	{
		scaleX *= 2;
		scaleY *= 2;
		scaleZ *= 2;
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
	UIHealthComponent* uiHealth = registry.AddComponent<UIHealthComponent>(entity);
	uiElement->Setup("ExMenu/EmptyHealth", "", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("ExMenu/FullHealth", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
#endif

	//Model

	ModelSkeletonComponent* model = nullptr;

	if (eType == EnemyType::hellhound)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
		stat->lavaAccelFactor = 1.0f;
		stat->lavaAnimFactor = 1.0f;
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("PHDoggo.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<HellhoundBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.3f, EnemyType::hellhound);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(HELLHOUND);
		if (player)
		{
			player->killThreshold++;
		}
	}
	else if (eType == EnemyType::skeleton)
	{
		//registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("Skeleton.mdl"));
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Skeleton.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<SkeletonBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 0.9f, EnemyType::skeleton);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(SKELETON);
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
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("Eye.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<EyeBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.f, EnemyType::eye, false);
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
	else if (eType == EnemyType::tempBoss)
	{
		stat->hazardModifier = 0.0f;
		ModelBonelessComponent* mod = registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("Skeleton.mdl"));
		mod->shared.gammaCorrection = 1.5f;
		registry.AddComponent<TempBossBehaviour>(entity, 0, 0);
		TempBossBehaviour* tempBossComponent = registry.GetComponent<TempBossBehaviour>(entity);
		for (int i = 0; i < 5; i++)
			tempBossComponent->parts[i] = true; // this is needed, DO NOT TOUCH

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
		registry.AddComponent<LuciferBehaviour>(entity, 0, 0);
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
			model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("PHDoggo.mdl"));
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
		SetupEnemyCollisionBox(entity, 1.3f, EnemyType::frozenHellhound);
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

	ModelSkeletonComponent* model = registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerPlaceholder.mdl"));
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
	UIHealthComponent* uiHealth = registry.AddComponent<UIHealthComponent>(stateManager.player);

	//Souls
	uiElement->AddImage("ExMenu/EmptyHealth", DSFLOAT2(-0.8f, 0.6f));
	uiElement->AddText("",uiElement->m_Images[0].baseUI.GetOriginalBounds(), DSFLOAT2(-0.8f, 0.6f));
	UIPlayerSoulsComponent* uiSouls = registry.AddComponent<UIPlayerSoulsComponent>(stateManager.player);
	
	//Relics
	uiElement->AddImage("TempRelicHolder11", DSFLOAT2(-0.95f, -0.1f));
	UIPlayerRelicsComponent* uiRelics = registry.AddComponent<UIPlayerRelicsComponent>(stateManager.player);
	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stateManager.player);

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
		modelLoaded= registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerPlaceholder.mdl"));
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
		playerTransform->mass = 3.0f;
	}
	playerTransform->currentSpeedX = 0.0f;
	playerTransform->currentSpeedZ = 0.0f;

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
}

EntityID RandomPlayerEnemy(EnemyType enemyType) {
	EntityID enemy = SetupEnemy(enemyType, (float)(rand() % 100) - 50.0f, 0.f, (float)(rand() % 100) - 50.0f);
	SetHitboxIsPlayer(enemy, 1, true);
	registry.AddComponent<PlayerComponent>(enemy);
	StatComponent* stats = registry.GetComponent<StatComponent>(enemy);
	return enemy;
}