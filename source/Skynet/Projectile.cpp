#include "Skynet/BehaviourHelper.h"
#include "Registry.h"
#include "Model.h"
#include "EventFunctions.h"

void CreateProjectile(EntityID entity, float directionX, float directionZ, EnemyType type)
{
	EntityID bullet = registry.CreateEntity();

	//Transform
	TransformComponent* origin = registry.GetComponent<TransformComponent>(entity);
	TransformComponent transform;
	transform.mass = 1.0f;
	//Direction
	transform.facingX = directionX;
	transform.facingY = 0;
	transform.facingZ = directionZ;
		
	float dist = sqrtf((transform.facingX * transform.facingX) + (transform.facingZ * transform.facingZ));
	transform.facingX /= dist;
	transform.facingZ /= dist;

	//Position
	transform.positionX = origin->positionX;
	transform.positionY = 5.0f;
	transform.positionZ = origin->positionZ;
	
//	if(type == imp)
//		transform.positionY = 5.0f;
//	else
//		transform.positionY = 1.0f;

	//Scale
	float scaleFactor = 6.0f;
	transform.scaleX = scaleFactor;
	transform.scaleY = scaleFactor;
	transform.scaleZ = scaleFactor;

	registry.AddComponent<TransformComponent>(bullet, transform);

	//Add negative light as a shadow for projectile clarity
		
	AddTimedEventComponentStartEnd(bullet, 0.0f, SpawnProjectileShadow, 1.01f, nullptr, 0, 2);

	registry.AddComponent<ProjectileComponent>(bullet, type, entity);

	//Stats needed in order to deal damage
	//Health, Speed, Damage, attackSpeed
	float speed = 30.0f;
	if (type == eye)
	{
		speed = 50.0f;
	}
	StatComponent* statsParent = registry.GetComponent<StatComponent>(entity);

	float damage = statsParent->GetDamage();
	StatComponent* stats = registry.AddComponent<StatComponent>(bullet, 1.0f, speed, damage, 0.0f);
		
	stats->UpdateBonusKnockback(2.0f);
	stats->baseHazardModifier = 0.0f;
	stats->hazardModifier = 0.0f;

	//Setup Bullet hitbox
	SetupProjectileCollisionBox(bullet, 1.2f);
	if (type == eye)
	{
		//VFX of the bullet
		registry.AddComponent<ParticleComponent>(bullet, 2.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 2, VFX_PATTERN::ACID);

		AddTimedEventComponentStartEnd(bullet, 1.0f, BeginDestroyProjectile, 1.01f, EndDestroyProjectile, 0, 2);
	}
	else if (type == empoweredImp)
	{
		//VFX of the bullet

		registry.AddComponent<ParticleComponent>(bullet, 2.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 2, VFX_PATTERN::FIREBALL_EMPOWERED);

		AddTimedEventComponentStartEnd(bullet, 5.0f, BeginDestroyProjectile, 5.01f, EndDestroyProjectile, 0, 2);
	}
	else
	{

		//VFX of the bullet
		
		registry.AddComponent<ParticleComponent>(bullet, 2.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 2, VFX_PATTERN::FIREBALL);

		AddTimedEventComponentStartEnd(bullet, 5.0f, BeginDestroyProjectile, 5.01f, EndDestroyProjectile, 0, 2);
	}
	
}