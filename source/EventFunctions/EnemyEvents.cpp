#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UIRenderer.h"
#include "CollisionFunctions.h" //AttackCollision
#include "Model.h"
#include "Levels\LevelHelper.h"
#include "Skynet\BehaviourHelper.h"
#include "UIComponents.h"

#define BOSS_RESPAWN_TIME 8.f;



//void EnemyExclusion(EntityID& entity)
//{
//	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
//	stat->performingDeathAnimation = true; //so we don't trigger death animation twice
//} not needed

void PlayDeathAnimation(EntityID& entity, const int& index)
{
	RemoveHitbox(entity, 0);
	RemoveHitbox(entity, 1);
	RemoveHitbox(entity, 2);
	auto transform = registry.GetComponent<TransformComponent>(entity);
	float offset = float(rand() % 2);
	offset -= 0.5f;
	transform->positionX += offset * 0.02f;
	offset = float(rand() % 2);
	offset -= 0.5f;
	transform->positionZ += offset * 0.02f;

	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	if (anim)
	{
		anim->aAnim = ANIMATION_DEATH;
		anim->aAnimIdx = 0;
		anim->aAnimTime = GetTimedEventElapsedTime(entity, index);
	}
	ParticleComponent* pc = registry.GetComponent<ParticleComponent>(entity);
	if (pc != nullptr)
	{
		pc->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}

	//Temp: Remove the light if dog dies during its flamethrower attack
	RemoveLight(entity);
	EnemyComponent* enmComp = registry.GetComponent<EnemyComponent>(entity);
	SetHitboxActive(entity, enmComp->specialHitBoxID, false);
	SetHitboxCanDealDamage(entity, enmComp->specialHitBoxID, false);


}

void CreateMini(const EntityID& original, const float xSpawn, const float zSpawn, const int zacIndex, const float health)
{	
	EntityID newMini = registry.CreateEntity();


	//Get components of original boss
	auto transform = registry.GetComponent<TransformComponent>(original);
	auto bossStats = registry.GetComponent<StatComponent>(original);
	auto bossBehev = registry.GetComponent<TempBossBehaviour>(original);

	
	
	

	// now we need the speed for the blob, to make sure it ends up in middle after X seconds
	float dista = Calculate2dDistance(xSpawn, zSpawn, transform->positionX, transform->positionZ);
	//float dista = 15.f;
	float speeeeeed = dista / BOSS_RESPAWN_TIME;

	//Set stats of new boss based on original
	//float bossHP = bossStats->GetMaxHealth() / 2.f;
	float bossSpeed = speeeeeed /*bossStats->GetSpeed() / 2.f */;
	float bossDamage = bossStats->GetDamage() / 2.f;
	float bossAttackSpeed = bossStats->GetAttackSpeed();
	StatComponent* stat = registry.AddComponent<StatComponent>(newMini, (health / 2.f), bossSpeed, bossDamage, bossAttackSpeed );
	// change health depending on balance. health = original max health
	stat->hazardModifier = 0;
	stat->baseHazardModifier = 0;
	stat->lavaAccelFactor = 1.f;
	stat->lavaAnimFactor = 1.f;
	//Set transform
	TransformComponent transComp;
	float newScaleSize = 0.3f; // change as see fit
	//float offsetX = transform->facingX;
	//float offsetZ = -transform->facingZ;
	//float magnitude = sqrtf(offsetX * offsetX + offsetZ * offsetZ);
	//if (magnitude > 0.001f)
	//{
	//	offsetX /= magnitude;
	//	offsetZ /= magnitude;
	//}
	transComp.positionX = xSpawn;
	transComp.positionZ = zSpawn;
	transComp.scaleX = transform->scaleX * newScaleSize;
	transComp.scaleY = transform->scaleY * newScaleSize;
	transComp.scaleZ = transform->scaleZ * newScaleSize;
	
	
	transComp.mass = transform->mass * 0.8f;
	registry.AddComponent<TransformComponent>(newMini, transComp); 
	int soulWorth = 2;

	registry.AddComponent<EnemyComponent>(newMini, soulWorth, -1);
	registry.AddComponent<ModelBonelessComponent>(newMini, LoadModel("Skeleton.mdl"));

#ifdef DEBUG_HP
	// UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(newMini);
	UIGameHealthComponent* uiHealth = registry.AddComponent<UIGameHealthComponent>(newMini);
	uiElement->Setup("ExMenu/EmptyHealth", "", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("ExMenu/FullHealth", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
#endif

	////Set hitbox
	//float newScaleHitBox = 0.9f;
	//float mini = 1.f;
	//if (deathC >= 2)
	//{
	//	mini = 1.4f;
	//}
	//else if (deathC == 3)
	//{
	//	newScaleHitBox = 1.2f;
	//	mini = 1.5f;
	//}
	
	//SetupEnemyCollisionBox(newMini, GetHitboxRadius(original, bossBehev->hitBoxID) * newScaleHitBox);

	float radius = GetHitboxRadius(original, bossBehev->hitBoxID) /** newScaleHitBox*/;

	AddHitboxComponent(newMini);
	EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(newMini);
	int hID = CreateHitbox(newMini, radius * /*mini **/ 0.7f, 0.f, 0.f);
	SetCollisionEvent(newMini, hID, HardCollision);
	SetHitboxIsEnemy(newMini, hID);
	SetHitboxHitPlayer(newMini, hID);
	SetHitboxHitEnemy(newMini, hID);
	SetHitboxActive(newMini, hID);
	SetHitboxIsMoveable(newMini, hID);

	int sID = CreateHitbox(newMini, radius, 0.f, 0.f);
	SetCollisionEvent(newMini, sID, SoftCollision);
	SetHitboxIsEnemy(newMini, sID);
	SetHitboxHitPlayer(newMini, sID);
	SetHitboxHitEnemy(newMini, sID);
	SetHitboxActive(newMini, sID);
	SetHitboxIsMoveable(newMini, sID);
	SetHitboxHitStaticHazard(newMini, sID, true);
	SetHitboxCanTakeDamage(newMini, sID);

	SetHitboxCanDealDamage(newMini, sID, false);

	enemyComp->attackHitBoxID = CreateHitbox(newMini, radius * 1.5f, 0.f, -1.5f);
	SetCollisionEvent(newMini, enemyComp->attackHitBoxID, AttackCollision);
	//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
	SetHitboxHitPlayer(newMini, enemyComp->attackHitBoxID);
	SetHitboxActive(newMini, enemyComp->attackHitBoxID);
	SetHitboxIsMoveable(newMini, enemyComp->attackHitBoxID);
	SetHitboxCanTakeDamage(newMini, enemyComp->attackHitBoxID, false);
	SetHitboxCanDealDamage(newMini, enemyComp->attackHitBoxID, false);


	registry.AddComponent<ZacBehaviour>(newMini, zacIndex, transform->positionX, transform->positionZ);
	
}

void CreateNewSplitZac(EntityID &ent, const int& index)
{
	TransformComponent* zacTransform = nullptr;
	zacTransform = registry.GetComponent<TransformComponent>(ent);
	bool zacIndex[5] = { false, false, false, false, false };
	bool shouldSpawn = false;
	for (auto enemyEntity : View<ZacBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		StatComponent* enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		if (enemyStats->GetHealth() > 0)
		{
			ZacBehaviour* zacComponent = registry.GetComponent<ZacBehaviour>(enemyEntity);
			zacIndex[zacComponent->zacIndex] = true;
			EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(enemyEntity);
			enemyComp->soulCount = 0;
			RemoveEnemy(enemyEntity, 69);
			shouldSpawn = true;
		}
	}
	
	if (shouldSpawn)
	{
		SetupEnemy(EnemyType::tempBoss, zacTransform->positionX, 0.f, zacTransform->positionZ, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 2.f, 2.f, 2.f,
			1.f, 0.f, 1.f, zacIndex[0], zacIndex[1], zacIndex[2], zacIndex[3], zacIndex[4]);
	}


	registry.DestroyEntity(ent);
}

void SplitBoss(EntityID& entity, const int& index)
{
	float radius = 30.f;
	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));
	CalculateGlobalMapValuesImp(valueGrid);
	TransformComponent* aiTransform = nullptr;
	aiTransform = registry.GetComponent<TransformComponent>(entity);
	TempBossBehaviour* tempBossComponent = nullptr;
	tempBossComponent = registry.GetComponent<TempBossBehaviour>(entity);
	StatComponent* originalStats = registry.GetComponent<StatComponent>(entity);

	float health = 0.f;
	int partsAlive = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (tempBossComponent->parts[i])
		{
			partsAlive++;
		}
	}
	health = originalStats->GetMaxHealth();
	health = health / (float)partsAlive * 5.f;

	for (int i = 0; i < 5; ++i)
	{
		if (tempBossComponent->parts[i] )
		{
			TransformComponent tran = FindRetreatTile(valueGrid, aiTransform, 25.f, 45.f);
			/*float angle = i * (2 * 3.141592 / 5);
			float x = radius * cos(angle);
			float y = radius * sin(angle);*/
			CreateMini(entity, tran.positionX, tran.positionZ, i, health);
			CalculateGlobalMapValuesImp(valueGrid);
		}
	}



	/*CreateMini(entity, 5.f * multiplier, 0.f * multiplier, 0);
	CreateMini(entity, -5.f * multiplier, -2.f * multiplier, 1);
	CreateMini(entity, 0.f * multiplier, -5.f * multiplier, 2);
	CreateMini(entity, 2.f * multiplier, 5.f * multiplier, 3);
	CreateMini(entity, -5.f * multiplier, 2.f * multiplier, 4);*/


	free(valueGrid);
	EntityID trashEntity = registry.CreateEntity();
	TransformComponent* transformZac = registry.AddComponent<TransformComponent>(trashEntity);
	transformZac->positionX = aiTransform->positionX;
	transformZac->positionZ = aiTransform->positionZ;
	float time = (float)BOSS_RESPAWN_TIME;
	AddTimedEventComponentStart(trashEntity, time - 0.5f, CreateNewSplitZac);

	RemoveEnemy(entity, index);
}

void BossShockwaveStart(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	SetHitboxActive(entity, enemy->specialHitBoxID, true);//Set false somewhere
	SetHitboxCanDealDamage(entity, enemy->specialHitBoxID, true);
	SetHitboxRadius(entity, enemy->specialHitBoxID, 0.0f);
}

void BossShockwaveExpand(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	float radius = GetHitboxRadius(entity, enemy->specialHitBoxID);
	float growthSpeed = 30.0f;
	radius += GetDeltaTime() * growthSpeed;
	SetHitboxRadius(entity, enemy->specialHitBoxID, radius);
}
void BossShockwaveEnd(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	SetHitboxActive(entity, enemy->specialHitBoxID, false);//Set false somewhere
	SetHitboxCanDealDamage(entity, enemy->specialHitBoxID, false);
}

void RemoveEnemy(EntityID& entity, const int& index)
{


	// Eat them souls
	for (auto player : View<PlayerComponent>(registry))
	{
		PlayerComponent* pc = registry.GetComponent<PlayerComponent>(player);
		EnemyComponent* ec = registry.GetComponent<EnemyComponent>(entity);
		pc->UpdateSouls(ec->soulCount);
	}
	
	// I am inevitable 
	// *le snap*
	auto toAppend = registry.GetComponent<ModelBonelessComponent>(entity);
	if (toAppend != nullptr)
	{
		ReleaseModel(toAppend->model);
		registry.RemoveComponent<ModelBonelessComponent>(entity);
	}


	auto toAppend2 = registry.GetComponent<ModelSkeletonComponent>(entity);
	if (toAppend2 != nullptr)
	{
		ReleaseModel(toAppend2->model);
		registry.RemoveComponent<ModelSkeletonComponent>(entity); 
	}

	SoundComponent* s = registry.GetComponent<SoundComponent>(entity);
	if (s != nullptr)
	{
		s->Unload();
	}

	TimedEventComponent* timed = registry.GetComponent<TimedEventComponent>(entity);
	if (timed != nullptr)
	{
		ReleaseTimedEvents(entity);
	}

	registry.DestroyEntity(entity);
}

void SpawnMainMenuEnemy(EntityID& entity, const int& index)
{
	int condition = GetTimedEventCondition(entity, index);
	switch (condition)
	{
	case invalidType:
		break;
	case hellhound:
		RandomPlayerEnemy(hellhound);
		break;
	case skeleton:
		RandomPlayerEnemy(skeleton);
		break;
	case eye:
		RandomPlayerEnemy(eye);
		break;
	case imp:
		RandomPlayerEnemy(imp);
		break;
	case tempBoss:
		RandomPlayerEnemy(tempBoss);
		break;
	default:
		break;
	}
}

void LoopSpawnMainMenuEnemy(EntityID& entity, const int& index)
{
	int rarity = 0;
	EnemyType type = skeleton;
	rarity = rand() % 16;
	if (rarity == 0)
	{
		type = hellhound;
	}
	rarity = rand() % 16;
	if (rarity == 0)
	{
		type = imp;
	}
	rarity = rand() % 64;
	if (rarity == 0)
	{
		type = eye;
	}
	rarity = rand() % 4096;
	if (rarity == 0)
	{
		type = tempBoss;
	}
	float time = 0.05f * (float)(rand() % 64);
	AddTimedEventComponentStartEnd(entity, 0.0f, SpawnMainMenuEnemy,time + 0.1f, LoopSpawnMainMenuEnemy, (unsigned)type, 2);
}

void DestroyAcidHazard(EntityID& entity, const int& index)
{
	if (entity.isDestroyed == true)
	{
		return;
	}
	ModelBonelessComponent* model = registry.GetComponent<ModelBonelessComponent>(entity);
	if (model != nullptr)
	{
		ReleaseModel(model->model);
		registry.RemoveComponent<ModelBonelessComponent>(entity);
	}

	RemoveHitbox(entity, 0);
	registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
}

void CreateAcidHazard(EntityID& entity, const int& index)
{
	TransformComponent* origin = registry.GetComponent<TransformComponent>(entity);
	
	EntityID acidHazard = registry.CreateEntity();
	ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(acidHazard, LoadModel("LavaPlaceholder.mdl"));
	hazardModel->shared.colorAdditiveRed = 0.1f;
	hazardModel->shared.colorAdditiveGreen = 0.9f;
	hazardModel->shared.colorAdditiveBlue = 0.2f;
	hazardModel->shared.gammaCorrection = 1.5f;
	hazardModel->castShadow = false;

	float scaling = 5.0f;

	TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(acidHazard);
	hazardTransform->positionX = origin->positionX;
	hazardTransform->positionY = 0.2f;
	hazardTransform->positionZ = origin->positionZ;
	hazardTransform->scaleX = scaling;
	hazardTransform->scaleY = 1.0f;
	hazardTransform->scaleZ = scaling;
	hazardTransform->facingX = cosf((float)rand());
	hazardTransform->facingZ = sinf((float)rand());
	AddStaticHazard(acidHazard, HAZARD_LAVA);

	registry.AddComponent<StaticHazardComponent>(acidHazard, StaticHazardType::HAZARD_ACID);

	float radius = 5.0f;
	AddHitboxComponent(acidHazard);
	int hitboxID = CreateHitbox(acidHazard, radius * 0.5f, 0.f, 0.f);
	SetCollisionEvent(acidHazard, hitboxID, HazardAttackCollision);
	SetHitboxHitPlayer(acidHazard, hitboxID);
	SetHitboxHitEnemy(acidHazard, hitboxID);
	SetHitboxActive(acidHazard, hitboxID);
	SetHitboxCanDealDamage(acidHazard, hitboxID, true);
	

	AddTimedEventComponentStart(acidHazard, 5.0f, DestroyAcidHazard);
	//AddTimedEventComponentStartEnd(acidHazard, 5.0f, DestroyAcidHazard, 5.5f, EndDestroyProjectile, 0, 2);
}

void BeginDestroyProjectile(EntityID& entity, const int& index)
{
	ProjectileComponent* proj = registry.GetComponent<ProjectileComponent>(entity);
	
	if (entity.isDestroyed == true)
	{
		return;
	}
	ModelBonelessComponent* model = registry.GetComponent<ModelBonelessComponent>(entity);
	if (model != nullptr)
	{
		ReleaseModel(model->model);
		registry.RemoveComponent<ModelBonelessComponent>(entity);
	}
	
	if (proj->type == eye)
	{
		CreateAcidHazard(entity, index);
		proj->type = imp;
	}
		

	RemoveHitbox(entity, 0);
	RemoveHitbox(entity, 1);
}

void EndDestroyProjectile(EntityID& entity, const int& index)
{
	if (entity.isDestroyed == true)
	{
		return;
	}
	registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
}
