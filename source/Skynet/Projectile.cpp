#include "Skynet/BehaviourHelper.h"
#include "Registry.h"
#include "Model.h"

void CreateProjectile(EntityID entity, float directionX, float directionZ)
{
	EntityID bullet = registry.CreateEntity();

	//Model of the bullet
	registry.AddComponent<ModelBonelessComponent>(bullet, LoadModel("AcidPlaceholder.mdl"));

	TransformComponent* origin = registry.GetComponent<TransformComponent>(entity);

	//Transform
	TransformComponent transform;
	transform.mass = 1.0f;
	
	//Direction
	transform.facingX = directionX; 
	transform.facingY = 0;
	transform.facingZ = directionZ;
	
	//Position
	transform.positionX = origin->positionX; 
	transform.positionY = 1.0f; 
	transform.positionZ = origin->positionZ;
	
	//Scale
	float scaleFactor = 6.0f;
	transform.scaleX = scaleFactor; 
	transform.scaleY = scaleFactor; 
	transform.scaleZ = scaleFactor;

	registry.AddComponent<TransformComponent>(bullet, transform);

	registry.AddComponent<ProjectileComponent>(bullet, 0, entity);

	//Stats needed in order to deal damage
	//Health, Speed, Damage, attackSpeed
	registry.AddComponent<StatComponent>(bullet, 1.0f, 40.0f, 5.0f, 0.0f);

	
	//Setup Bullet hitbox
	SetupProjectileCollisionBox(bullet, 1.0f);
}