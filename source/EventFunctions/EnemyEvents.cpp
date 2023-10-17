#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UIRenderer.h"
#include "CollisionFunctions.h" //AttackCollision

//void EnemyExclusion(EntityID& entity)
//{
//	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
//	stat->performingDeathAnimation = true; //so we don't trigger death animation twice
//} not needed

void PlayDeathAnimation(EntityID& entity)
{
	//implement later, goddamn TA

	auto transform = registry.GetComponent<TransformComponent>(entity);
	float offset = float(rand() % 2);
	offset -= 0.5f;
	transform->positionX += offset * 0.02f;
	offset = float(rand() % 2);
	offset -= 0.5f;
	transform->positionZ += offset * 0.02f;
}

void SplitBoss(EntityID& entity)
{
	auto transform = registry.GetComponent<TransformComponent>(entity);
	auto bossStats = registry.GetComponent<StatComponent>(entity);
	auto bossBehev = registry.GetComponent<TempBossBehaviour>(entity);

	float newScaleSize = 0.7f; // change as see fit
	float newScaleHitBox = 0.85f;
	bossBehev->deathCounter = bossBehev->deathCounter + 1;
	float deathC = bossBehev->deathCounter;

	EntityID tempBoss = registry.CreateEntity();
	EntityID tempBoss2 = registry.CreateEntity();


	float bossHP = bossStats->maxHealth / 2.f;
	float bossSpeed = bossStats->moveSpeed;
	float bossDamage = bossStats->damage / 2.f;
	float bossAttackSpeed = bossStats->attackSpeed;

	float offsetX = transform->facingX;
	float offsetZ = -transform->facingZ;
	float offsetValue = 3.f;
	float magnitude = sqrt(offsetX * offsetX + offsetZ * offsetZ);
	if (magnitude > 0.001f)
	{
		offsetX /= magnitude;
		offsetZ /= magnitude;
	}
	// First tempBoss
	TransformComponent fsTransformComponent;
	fsTransformComponent.positionX = transform->positionX + offsetX * offsetValue;
	fsTransformComponent.positionZ = transform->positionZ + offsetZ * offsetValue;
	fsTransformComponent.scaleX = transform->scaleX * newScaleSize;
	fsTransformComponent.scaleY = transform->scaleY * newScaleSize;
	fsTransformComponent.scaleZ = transform->scaleZ * newScaleSize;
	

	// Second tempBoss2
	TransformComponent ssTransformComponent;
	ssTransformComponent.positionX = transform->positionX - offsetX * offsetValue;
	ssTransformComponent.positionZ = transform->positionZ - offsetZ * offsetValue;
	ssTransformComponent.scaleX = transform->scaleX * newScaleSize;
	ssTransformComponent.scaleY = transform->scaleY * newScaleSize;
	ssTransformComponent.scaleZ = transform->scaleZ * newScaleSize;


	AddHitboxComponent(tempBoss);

	int hID = CreateHitbox(tempBoss, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	SetCollisionEvent(tempBoss, hID, HardCollision);
	SetHitboxIsEnemy(tempBoss, hID);
	SetHitboxHitPlayer(tempBoss, hID);
	SetHitboxHitEnemy(tempBoss, hID);
	SetHitboxActive(tempBoss, hID);
	SetHitboxIsMoveable(tempBoss, hID);

	int sID = CreateHitbox(tempBoss, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	SetCollisionEvent(tempBoss, sID, SoftCollision);
	SetHitboxIsEnemy(tempBoss, sID);
	SetHitboxHitPlayer(tempBoss, sID);
	SetHitboxHitEnemy(tempBoss, sID);
	SetHitboxActive(tempBoss, sID);
	SetHitboxIsMoveable(tempBoss, sID);



	AddHitboxComponent(tempBoss2);

	int hID2 = CreateHitbox(tempBoss2, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	SetCollisionEvent(tempBoss2, hID2, HardCollision);
	SetHitboxIsEnemy(tempBoss2, hID2);
	SetHitboxHitPlayer(tempBoss2, hID2);
	SetHitboxHitEnemy(tempBoss2, hID2);
	SetHitboxActive(tempBoss2, hID2);
	SetHitboxIsMoveable(tempBoss2, hID2);

	int sID2 = CreateHitbox(tempBoss2, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScaleHitBox, 0.f, 0.f);
	SetCollisionEvent(tempBoss2, sID2, SoftCollision);
	SetHitboxIsEnemy(tempBoss2, sID2);
	SetHitboxHitPlayer(tempBoss2, sID2);
	SetHitboxHitEnemy(tempBoss2, sID2);
	SetHitboxActive(tempBoss2, sID2);
	SetHitboxIsMoveable(tempBoss2, sID2);




	registry.AddComponent<TransformComponent>(tempBoss, fsTransformComponent);
	registry.AddComponent<TransformComponent>(tempBoss2, ssTransformComponent);

	registry.AddComponent<StatComponent>(tempBoss, bossHP, bossSpeed, bossDamage, bossAttackSpeed);
	registry.AddComponent<EnemyComponent>(tempBoss, 2);

	registry.AddComponent<StatComponent>(tempBoss2, bossHP, bossSpeed, bossDamage, bossAttackSpeed);
	registry.AddComponent<EnemyComponent>(tempBoss2, 2);

	registry.AddComponent<ModelBonelessComponent>(tempBoss, LoadModel("PHBoss.mdl"));
	registry.AddComponent<ModelBonelessComponent>(tempBoss2, LoadModel("PHBoss.mdl"));
	


	registry.AddComponent<TempBossBehaviour>(tempBoss, deathC, hID);
	registry.AddComponent<TempBossBehaviour>(tempBoss2, deathC, hID2);

	RemoveEnemy(entity);
}


void RemoveEnemy(EntityID& entity)
{
	// I am inevitable 
	// *le snap*
	CREATE_ENTITY_MAP_entities;
	auto toAppend = registry.GetComponent<ModelBonelessComponent>(entity);
	if (toAppend != nullptr)
		ReleaseModel(toAppend->model);
	ADD_TO_entities_IF_NOT_INCLUDED(entity);


	auto toAppend2 = registry.GetComponent<TransformComponent>(entity);
	if (toAppend2 != nullptr)
		registry.RemoveComponent<TransformComponent>(entity);
	ADD_TO_entities_IF_NOT_INCLUDED(entity);
	


	auto toAppend3 = registry.GetComponent<ModelSkeletonComponent>(entity);
	if (toAppend3 != nullptr)
		registry.RemoveComponent<ModelSkeletonComponent>(entity);
	ADD_TO_entities_IF_NOT_INCLUDED(entity);
	

	auto toAppend4 = registry.GetComponent<PointOfInterestComponent>(entity);
	if (toAppend4 != nullptr)
		registry.RemoveComponent<PointOfInterestComponent>(entity);
	ADD_TO_entities_IF_NOT_INCLUDED(entity);
	

	auto toAppend5 = registry.GetComponent<EnemyComponent>(entity);
	if (toAppend5 != nullptr)
		registry.RemoveComponent<EnemyComponent>(entity);
	ADD_TO_entities_IF_NOT_INCLUDED(entity);
	

	auto toAppend6 = registry.GetComponent<StatComponent>(entity);
	if (toAppend6 != nullptr)
		registry.RemoveComponent<StatComponent>(entity);
	ADD_TO_entities_IF_NOT_INCLUDED(entity)


	uint16_t destCount = DestroyEntities(entities);


	RedrawUI();
}