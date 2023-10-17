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

	float newScale = 0.75f; // change as see fit

	EntityID tempBoss = registry.CreateEntity();
	EntityID tempBoss2 = registry.CreateEntity();
	registry.AddComponent<ModelBonelessComponent>(tempBoss, LoadModel("PHBoss.mdl"));
	registry.AddComponent<ModelBonelessComponent>(tempBoss2, LoadModel("PHBoss.mdl"));

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
	fsTransformComponent.scaleX = transform->scaleX * newScale;
	fsTransformComponent.scaleY = transform->scaleY * newScale;
	fsTransformComponent.scaleZ = transform->scaleZ * newScale;
	registry.AddComponent<TransformComponent>(tempBoss, fsTransformComponent);

	// Second tempBoss2
	TransformComponent ssTransformComponent;
	ssTransformComponent.positionX = transform->positionX - offsetX * offsetValue;
	ssTransformComponent.positionZ = transform->positionZ - offsetZ * offsetValue;
	ssTransformComponent.scaleX = transform->scaleX * newScale;
	ssTransformComponent.scaleY = transform->scaleY * newScale;
	ssTransformComponent.scaleZ = transform->scaleZ * newScale;
	registry.AddComponent<TransformComponent>(tempBoss2, ssTransformComponent);

	registry.AddComponent<StatComponent>(tempBoss, bossStats->maxHealth, bossStats->moveSpeed, bossStats->damage, bossStats->attackSpeed);
	registry.AddComponent<EnemyComponent>(tempBoss, 2);

	registry.AddComponent<StatComponent>(tempBoss2, bossStats->maxHealth, bossStats->moveSpeed, bossStats->damage, bossStats->attackSpeed);
	registry.AddComponent<EnemyComponent>(tempBoss2, 2);

	bossBehev->deathCounter++;

	registry.AddComponent<TempBossBehaviour>(tempBoss, bossBehev->deathCounter, bossBehev->hitBoxID);
	registry.AddComponent<TempBossBehaviour>(tempBoss2, bossBehev->deathCounter, bossBehev->hitBoxID);

	

	(bossBehev->deathCounter == 1) ? newScale : newScale * newScale;

	SetupEnemyCollisionBox(tempBoss, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScale);
	SetupEnemyCollisionBox(tempBoss2, GetHitboxRadius(entity, bossBehev->hitBoxID) * newScale);

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