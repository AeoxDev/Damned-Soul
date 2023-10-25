#include "Levels\LevelHelper.h"
#include "Registry.h"
#include "Components.h"
#include "Hitbox.h"
#include "Model.h"
#include "States\StateManager.h"

EntityID SetupEnemy(enemyType eType, float positionX , float positionY , float positionZ , float mass ,
	float health , float moveSpeed , float damage, float attackSpeed , int soulWorth, float scaleX, float scaleY, float scaleZ, float facingX ,
	float facingY , float facingZ  )
{
	EntityID entity = registry.CreateEntity();
	TransformComponent transform;

	assert(mass > 0.0f);
	if (mass == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			mass = 1.f;
		}
		else if (eType == enemyType::hellhound)
		{
			mass = 6.f;
		}
		else if (eType == enemyType::tempBoss)
		{
			mass = 50.f;
		}
		else if (eType == enemyType::skeleton)
		{
			mass = 3.f;
		}
	}
	if (health == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			health = 60.f;
		}
		else if (eType == enemyType::hellhound)
		{
			health = 150.f;
		}
		else if (eType == enemyType::tempBoss)
		{
			health = 400.f;
		}
		else if (eType == enemyType::skeleton)
		{
			health = 100.f;
		}
	}
	if (moveSpeed == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			moveSpeed = 13.f;
		}
		else if (eType == enemyType::hellhound)
		{
			moveSpeed = 15.f;
		}
		else if (eType == enemyType::tempBoss)
		{
			moveSpeed = 10.f;
		}
		else if (eType == enemyType::skeleton)
		{
			moveSpeed = 10.f;
		}
	}
	if (damage == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			damage = 10.f;
		}
		else if (eType == enemyType::hellhound)
		{
			damage = 10.f;
		}
		else if (eType == enemyType::tempBoss)
		{
			damage = 20.f;
		}
		else if (eType == enemyType::skeleton)
		{
			damage = 5.f;
		}
	}
	if (attackSpeed == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			attackSpeed = 5.f;
		}
		else if (eType == enemyType::hellhound)
		{
			attackSpeed = 0.5f;
		}
		else if (eType == enemyType::tempBoss)
		{
			attackSpeed = 0.5f;
		}
		else if (eType == enemyType::skeleton)
		{
			attackSpeed = 0.5f;
		}
	}
	if (soulWorth == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			soulWorth = 2;
		}
		else if (eType == enemyType::hellhound)
		{
			soulWorth = 3;
		}
		else if (eType == enemyType::tempBoss)
		{
			soulWorth = 4;
		}
		else if (eType == enemyType::skeleton)
		{
			soulWorth = 1;
		}
	}
	if (soulWorth == 6969.f)
	{
		if (eType == enemyType::eye)
		{
			soulWorth = 2;
		}
		else if (eType == enemyType::hellhound)
		{
			soulWorth = 3;
		}
		else if (eType == enemyType::tempBoss)
		{
			soulWorth = 4;
		}
		else if (eType == enemyType::skeleton)
		{
			soulWorth = 1;
		}
	}

	transform.mass = mass;
	transform.facingX = facingX; transform.facingY = facingY; transform.facingZ = facingZ;
	transform.positionX = positionX; transform.positionY = positionY; transform.positionZ = positionZ;
	transform.scaleX = scaleX; transform.scaleY = scaleY; transform.scaleZ = scaleZ;
	registry.AddComponent<TransformComponent>(entity, transform);

	registry.AddComponent<StatComponent>(entity, health, moveSpeed, damage, attackSpeed);
	registry.AddComponent<EnemyComponent>(entity, soulWorth, eType);

	

	if (eType == enemyType::hellhound)
	{
		registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("PHDoggo.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<HellhoundBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.3f);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(HELLHOUND);
	}
	else if (eType == enemyType::skeleton)
	{
		registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("PHSkeleton.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<SkeletonBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 0.9f);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(SKELETON);
	}
	else if (eType == enemyType::eye)
	{
		registry.AddComponent<ModelSkeletonComponent>(entity, LoadModel("EyePlaceholder.mdl"));
		registry.AddComponent<AnimationComponent>(entity);
		registry.AddComponent<EyeBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.f, false);
		//Sounds
		SoundComponent* scp = registry.AddComponent<SoundComponent>(entity);
		scp->Load(EYE);
	}
	else if (eType == enemyType::tempBoss)
	{
		registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("PHBoss.mdl"));
		registry.AddComponent<TempBossBehaviour>(entity, 0, 0);
		SetupEnemyCollisionBox(entity, 1.4f * scaleX);
	}

	return entity;
}

void CreatePlayer(float positionX, float positionY, float positionZ, float mass, float health, float moveSpeed, float damage, float attackSpeed, int soulWorth, float facingX, float facingY, float facingZ, float scaleX, float scaleY, float scaleZ)
{
	//Create player
	stateManager.player = registry.CreateEntity();

	registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerPlaceholder.mdl"));
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
		registry.AddComponent<ModelSkeletonComponent>(stateManager.player, LoadModel("PlayerPlaceholder.mdl"));
	}
	AnimationComponent* animationLoaded = registry.GetComponent<AnimationComponent>(stateManager.player);
	if (animationLoaded == nullptr)
	{
		registry.AddComponent<AnimationComponent>(stateManager.player, AnimationComponent());
	}

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
}