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