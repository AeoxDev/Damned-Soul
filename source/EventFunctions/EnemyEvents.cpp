#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UIRenderer.h"
#include "CollisionFunctions.h" //AttackCollision
#include "Model.h"
#include "Levels\LevelHelper.h"

#include "UIComponents.h"


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

void CreateMini(const EntityID& original, const float offsetValue)
{	
	EntityID newMini = registry.CreateEntity();


	//Get components of original boss
	auto transform = registry.GetComponent<TransformComponent>(original);
	auto bossStats = registry.GetComponent<StatComponent>(original);
	auto bossBehev = registry.GetComponent<TempBossBehaviour>(original);

	//Set stats of new boss based on original
	float bossHP = bossStats->GetMaxHealth() / 2.f;
	float bossSpeed = bossStats->GetSpeed();
	float bossDamage = bossStats->GetDamage() / 2.f;
	float bossAttackSpeed = bossStats->GetAttackSpeed();
	registry.AddComponent<StatComponent>(newMini, bossHP, bossSpeed, bossDamage, bossAttackSpeed);

	//Set transform
	TransformComponent transComp;
	float newScaleSize = 0.7f; // change as see fit
	float offsetX = transform->facingX;
	float offsetZ = -transform->facingZ;
	float magnitude = sqrtf(offsetX * offsetX + offsetZ * offsetZ);
	if (magnitude > 0.001f)
	{
		offsetX /= magnitude;
		offsetZ /= magnitude;
	}
	transComp.positionX = transform->positionX + offsetX * offsetValue;
	transComp.positionZ = transform->positionZ + offsetZ * offsetValue;
	transComp.scaleX = transform->scaleX * newScaleSize;
	transComp.scaleY = transform->scaleY * newScaleSize;
	transComp.scaleZ = transform->scaleZ * newScaleSize;
	//Set behavior
	float deathC = (float)(bossBehev->deathCounter + 1);
	/*if (deathC >= 2)
	{
		transComp.mass = transform->mass * 0.005f;
	}
	else
	{
		transComp.mass = transform->mass * 0.8f;
	}*/
	transComp.mass = transform->mass * 0.8f;
	registry.AddComponent<TransformComponent>(newMini, transComp);
	registry.AddComponent<EnemyComponent>(newMini, 2, -1);
	registry.AddComponent<ModelBonelessComponent>(newMini, LoadModel("PHBoss.mdl"));

#ifdef DEBUG_HP
	// UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(newMini);
	UIHealthComponent* uiHealth = registry.AddComponent<UIHealthComponent>(newMini);
	uiElement->Setup("ExMenu/EmptyHealth", "", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("ExMenu/FullHealth", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
#endif

	////Set hitbox
	//float newScaleHitBox = 0.9f;
	float mini = 1.f;
	if (deathC >= 2)
	{
		mini = 1.4f;
	}
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

	int sID = CreateHitbox(newMini, radius * mini, 0.f, 0.f);
	SetCollisionEvent(newMini, sID, SoftCollision);
	SetHitboxIsEnemy(newMini, sID);
	SetHitboxHitPlayer(newMini, sID);
	SetHitboxHitEnemy(newMini, sID);
	SetHitboxActive(newMini, sID);
	SetHitboxIsMoveable(newMini, sID);
	SetHitboxHitStaticHazard(newMini, sID, true);
	SetHitboxCanTakeDamage(newMini, sID);

	SetHitboxCanDealDamage(newMini, sID, false);

	enemyComp->attackHitBoxID = CreateHitbox(newMini, radius * mini * 1.5f, 0.f, -1.5f);
	SetCollisionEvent(newMini, enemyComp->attackHitBoxID, AttackCollision);
	//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
	SetHitboxHitPlayer(newMini, enemyComp->attackHitBoxID);
	SetHitboxActive(newMini, enemyComp->attackHitBoxID);
	SetHitboxIsMoveable(newMini, enemyComp->attackHitBoxID);
	SetHitboxCanTakeDamage(newMini, enemyComp->attackHitBoxID, false);
	SetHitboxCanDealDamage(newMini, enemyComp->attackHitBoxID, false);


	registry.AddComponent<TempBossBehaviour>(newMini, (int)deathC, hID);
	
	
	
	
}

void SplitBoss(EntityID& entity, const int& index)
{
	CreateMini(entity, 5.f);
	CreateMini(entity, -5.f);

	//auto transform = registry.GetComponent<TransformComponent>(entity);
	//auto bossStats = registry.GetComponent<StatComponent>(entity);
	//auto bossBehev = registry.GetComponent<TempBossBehaviour>(entity);

	//float newScaleSize = 0.7f; // change as see fit
	//float newScaleHitBox = 0.85f;
	//bossBehev->deathCounter = bossBehev->deathCounter + 1;
	//float deathC = bossBehev->deathCounter;

	//EntityID tempBoss = registry.CreateEntity();
	//EntityID tempBoss2 = registry.CreateEntity();


	//float bossHP = bossStats->maxHealth / 2.f;
	//float bossSpeed = bossStats->moveSpeed;
	//float bossDamage = bossStats->damage / 2.f;
	//float bossAttackSpeed = bossStats->attackSpeed;

	//float offsetX = transform->facingX;
	//float offsetZ = -transform->facingZ;
	//float offsetValue = 3.f;
	//float magnitude = sqrt(offsetX * offsetX + offsetZ * offsetZ);
	//if (magnitude > 0.001f)
	//{
	//	offsetX /= magnitude;
	//	offsetZ /= magnitude;
	//}
	//// First tempBoss
	//TransformComponent fsTransformComponent;
	//fsTransformComponent.positionX = transform->positionX + offsetX * offsetValue;
	//fsTransformComponent.positionZ = transform->positionZ + offsetZ * offsetValue;
	//fsTransformComponent.scaleX = transform->scaleX * newScaleSize;
	//fsTransformComponent.scaleY = transform->scaleY * newScaleSize;
	//fsTransformComponent.scaleZ = transform->scaleZ * newScaleSize;
	//

	//// Second tempBoss2
	//TransformComponent ssTransformComponent;
	//ssTransformComponent.positionX = transform->positionX - offsetX * offsetValue;
	//ssTransformComponent.positionZ = transform->positionZ - offsetZ * offsetValue;
	//ssTransformComponent.scaleX = transform->scaleX * newScaleSize;
	//ssTransformComponent.scaleY = transform->scaleY * newScaleSize;
	//ssTransformComponent.scaleZ = transform->scaleZ * newScaleSize;


	//registry.AddComponent<TransformComponent>(tempBoss, fsTransformComponent);
	//registry.AddComponent<TransformComponent>(tempBoss2, ssTransformComponent);

	//AddHitboxComponent(tempBoss);

	//int hID = CreateHitbox(tempBoss, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	//SetCollisionEvent(tempBoss, hID, HardCollision);
	//SetHitboxIsEnemy(tempBoss, hID);
	//SetHitboxHitPlayer(tempBoss, hID);
	//SetHitboxHitEnemy(tempBoss, hID);
	//SetHitboxActive(tempBoss, hID);
	//SetHitboxIsMoveable(tempBoss, hID);

	//int sID = CreateHitbox(tempBoss, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	//SetCollisionEvent(tempBoss, sID, SoftCollision);
	//SetHitboxIsEnemy(tempBoss, sID);
	//SetHitboxHitPlayer(tempBoss, sID);
	//SetHitboxHitEnemy(tempBoss, sID);
	//SetHitboxActive(tempBoss, sID);
	//SetHitboxIsMoveable(tempBoss, sID);



	//AddHitboxComponent(tempBoss2);

	//int hID2 = CreateHitbox(tempBoss2, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	//SetCollisionEvent(tempBoss2, hID2, HardCollision);
	//SetHitboxIsEnemy(tempBoss2, hID2);
	//SetHitboxHitPlayer(tempBoss2, hID2);
	//SetHitboxHitEnemy(tempBoss2, hID2);
	//SetHitboxActive(tempBoss2, hID2);
	//SetHitboxIsMoveable(tempBoss2, hID2);

	//int sID2 = CreateHitbox(tempBoss2, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	//SetCollisionEvent(tempBoss2, sID2, SoftCollision);
	//SetHitboxIsEnemy(tempBoss2, sID2);
	//SetHitboxHitPlayer(tempBoss2, sID2);
	//SetHitboxHitEnemy(tempBoss2, sID2);
	//SetHitboxActive(tempBoss2, sID2);
	//SetHitboxIsMoveable(tempBoss2, sID2);




	//

	//registry.AddComponent<StatComponent>(tempBoss, bossHP, bossSpeed, bossDamage, bossAttackSpeed);
	//registry.AddComponent<EnemyComponent>(tempBoss, 2);

	//registry.AddComponent<StatComponent>(tempBoss2, bossHP, bossSpeed, bossDamage, bossAttackSpeed);
	//registry.AddComponent<EnemyComponent>(tempBoss2, 2);

	//registry.AddComponent<ModelBonelessComponent>(tempBoss, LoadModel("PHBoss.mdl"));
	//registry.AddComponent<ModelBonelessComponent>(tempBoss2, LoadModel("PHBoss.mdl"));
	//


	//registry.AddComponent<TempBossBehaviour>(tempBoss, deathC, hID);
	//registry.AddComponent<TempBossBehaviour>(tempBoss2, deathC, hID2);

	RemoveEnemy(entity, index);
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
	rarity = rand() % 8;
	if (rarity == 0)
	{
		type = hellhound;
	}
	rarity = rand() % 8;
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
	hazardModel->colorAdditiveRed = 0.1f;
	hazardModel->colorAdditiveGreen = 0.9f;
	hazardModel->colorAdditiveBlue = 0.2f;
	hazardModel->gammaCorrection = 1.5f;
	hazardModel->castShadow = false;

	float scaling = 5.0f;

	TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(acidHazard);
	hazardTransform->positionX = origin->positionX;
	hazardTransform->positionY = 0.5f;
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
	

	CreateAcidHazard(entity, index);

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
