#include "Levels\LevelHelper.h"
#include "Registry.h"
#include "Components.h"
#include "Hitbox.h"
#include "Model.h"
#include "EventFunctions.h"
#include "States\StateManager.h"

EntityID SetupEnemy(EnemyType eType, float positionX , float positionY , float positionZ , float mass ,
	float health , float moveSpeed , float damage, float attackSpeed , int soulWorth, float scaleX, float scaleY, float scaleZ, float facingX ,
	float facingY , float facingZ  )
{
	EntityID entity = registry.CreateEntity();
	TransformComponent transform;
	auto player = registry.GetComponent<PlayerComponent>(stateManager.player);
	

	assert(mass > 0.0f);
	if (mass == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			mass = 1.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			mass = 6.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			mass = 50.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			mass = 3.f;
		}
	}
	if (health == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			health = 60.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			health = 150.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			health = 400.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			health = 100.f;
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
		else if (eType == EnemyType::tempBoss)
		{
			moveSpeed = 10.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			moveSpeed = 10.f;
		}
	}
	if (damage == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			damage = 10.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			damage = 10.f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			damage = 20.f;
		}
		else if (eType == EnemyType::skeleton)
		{
			damage = 5.f;
		}
	}
	if (attackSpeed == 6969.f)
	{
		if (eType == EnemyType::eye)
		{
			attackSpeed = 5.f;
		}
		else if (eType == EnemyType::hellhound)
		{
			attackSpeed = 0.1f;
		}
		else if (eType == EnemyType::tempBoss)
		{
			attackSpeed = 0.5f;
		}
		else if (eType == EnemyType::skeleton)
		{
			attackSpeed = 0.5f;
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
		else if (eType == EnemyType::tempBoss)
		{
			soulWorth = 4;
		}
		else if (eType == EnemyType::skeleton)
		{
			soulWorth = 1;
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
		else if (eType == EnemyType::tempBoss)
		{
			soulWorth = 4;
		}
		else if (eType == EnemyType::skeleton)
		{
			soulWorth = 1;
		}
	}

	transform.mass = mass;
	transform.facingX = facingX; transform.facingY = facingY; transform.facingZ = facingZ;
	transform.positionX = positionX; transform.positionY = positionY; transform.positionZ = positionZ;
	transform.scaleX = scaleX; transform.scaleY = scaleY; transform.scaleZ = scaleZ;
	registry.AddComponent<TransformComponent>(entity, transform);

	StatComponent* stat = registry.AddComponent<StatComponent>(entity, health, moveSpeed, damage, attackSpeed);
	registry.AddComponent<EnemyComponent>(entity, soulWorth, eType);

	ModelSkeletonComponent* model = nullptr;

	if (eType == EnemyType::hellhound)
	{
		stat->hazardModifier = 0.0f;
		stat->baseHazardModifier = 0.0f;
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
		registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("Skeleton.mdl"));
		//model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("PHSkeleton.mdl"));
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
		model = registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("EyePlaceholder.mdl"));
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
	else if (eType == EnemyType::tempBoss)
	{
		stat->hazardModifier = 0.0f;
		ModelBonelessComponent* mod = registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("PHBoss.mdl"));
		mod->gammaCorrection = 1.5f;
		registry.AddComponent<TempBossBehaviour>(entity, 0, 0);
		SetupEnemyCollisionBox(entity, 1.4f * scaleX, EnemyType::tempBoss);
		if (player)
		{
			player->killThreshold+=15;
		}
	}
	if (model != nullptr)
	{
		model->gammaCorrection = 1.5f;
	}
	CreatePointLight(entity, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);
	return entity;
}

void CreatePlayer(float positionX, float positionY, float positionZ, float mass, float health, float moveSpeed, float damage, float attackSpeed, int soulWorth, float facingX, float facingY, float facingZ, float scaleX, float scaleY, float scaleZ)
{
	//Create player
	stateManager.player = registry.CreateEntity(ENT_PERSIST_LEVEL);

	ModelSkeletonComponent* model = registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerPlaceholder.mdl"));
	model->colorMultiplicativeRed = 1.25f;
	model->colorMultiplicativeGreen = 1.25f;
	model->colorMultiplicativeBlue = 1.25f;
	model->gammaCorrection = 1.5f;
	registry.AddComponent<AnimationComponent>(stateManager.player, AnimationComponent());

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
		modelLoaded->colorMultiplicativeRed = 1.25f;
		modelLoaded->colorMultiplicativeGreen = 1.25f;
		modelLoaded->colorMultiplicativeBlue = 1.25f;
		modelLoaded->gammaCorrection = 1.5f;
	}
	AnimationComponent* animationLoaded = registry.GetComponent<AnimationComponent>(stateManager.player);
	if (animationLoaded == nullptr)
	{
		animationLoaded = registry.AddComponent<AnimationComponent>(stateManager.player, AnimationComponent());
		
	}
	animationLoaded->aAnimTimeFactor = 1.0f;
	LoadPlayerSounds();

	// Player (Default)
	TransformComponent* playerTransform = registry.GetComponent<TransformComponent>(stateManager.player);
	if (playerTransform == nullptr)
	{
		playerTransform = registry.AddComponent<TransformComponent>(stateManager.player);
		playerTransform->mass = 3.0f;
	}

	ControllerComponent* controller = registry.GetComponent<ControllerComponent>(stateManager.player);
	if (controller == nullptr)
	{
		registry.AddComponent<ControllerComponent>(stateManager.player);
	}
	PointOfInterestComponent* cameraPoint = registry.GetComponent<PointOfInterestComponent>(stateManager.player);
	if (cameraPoint == nullptr)
	{
		cameraPoint = registry.AddComponent<PointOfInterestComponent>(stateManager.player);
		cameraPoint->weight = 10.0f;
	}
	
	SetupPlayerCollisionBox(stateManager.player, 1.0f);
	MouseComponentAddComponent(stateManager.player);

	int squashStretch1 = AddTimedEventComponentStart(stateManager.player, 0.0f, ResetSquashStretch);
	CreatePointLight(stateManager.player, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);
}

EntityID RandomPlayerEnemy(EnemyType enemyType) {
	EntityID enemy = SetupEnemy(enemyType, (float)(rand() % 100) - 50.0f, 0.f, (float)(rand() % 100) - 50.0f);
	SetHitboxIsPlayer(enemy, 1, true);
	registry.AddComponent<PlayerComponent>(enemy);
	StatComponent* stats = registry.GetComponent<StatComponent>(enemy);
	return enemy;
}