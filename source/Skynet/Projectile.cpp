#include "Skynet/BehaviourHelper.h"
#include "Registry.h"
#include "Model.h"

#define PI 3.14159265

EntityID& CreateProjectile(TransformComponent* origin, float directionX, float directionZ)
{
	EntityID bullet = registry.CreateEntity();

	//Model of the bullet
	registry.AddComponent<ModelBonelessComponent>(bullet, LoadModel("AcidPlaceholder.mdl"));

	//Transform
	TransformComponent transform;
	transform.mass = 1.0f;
	
	//Direction
	transform.facingX = origin->facingX; 
	transform.facingY = 0;
	transform.facingZ = origin->facingZ;
	
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

	registry.AddComponent<ProjectileComponent>(bullet, 0);

	//Stats needed in order to deal damage
	//Health, Speed, Damage, attackSpeed
	registry.AddComponent<StatComponent>(bullet, 1.0f, 20.0f, 5.0f, 0.0f);

	

	//Setup Bullet hitbox
	SetupProjectileCollisionBox(bullet, 1.0f);

	return bullet;
}