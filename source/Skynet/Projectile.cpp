#include "Skynet/BehaviourHelper.h"
#include "Registry.h"
#include "Model.h"
#include "EventFunctions.h"

void CreateProjectile(EntityID entity, float directionX, float directionZ)
{
	int nrProjectiles = 1;//Get number of projectiles from component?
	for (size_t i = 0; i < nrProjectiles; i++)
	{
		EntityID bullet = registry.CreateEntity();

		//Model of the bullet
		registry.AddComponent<ModelBonelessComponent>(bullet, LoadModel("AcidPlaceholder.mdl"));

		TransformComponent* origin = registry.GetComponent<TransformComponent>(entity);

		//Transform
		TransformComponent transform;
		transform.mass = 1.0f;
		//Direction
		transform.facingX = directionX + (((float)(rand() % 8) * 0.1f - 0.35f) * (float)(nrProjectiles > 1));
		transform.facingY = 0;
		transform.facingZ = directionZ + (((float)(rand() % 8) * 0.1f - 0.35f) * (float)(nrProjectiles > 1));
		float dist = sqrtf((transform.facingX * transform.facingX) + (transform.facingZ * transform.facingZ));
		transform.facingX /= dist;
		transform.facingZ /= dist;

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
		StatComponent* stats = registry.AddComponent<StatComponent>(bullet, 1.0f, 25.0f, 15.0f, 0.0f);
		stats->UpdateBonusKnockback(2.0f);
		stats->baseHazardModifier = 0.0f;
		stats->hazardModifier = 0.0f;

		//Setup Bullet hitbox
		SetupProjectileCollisionBox(bullet, 1.0f);
		AddTimedEventComponentStartEnd(bullet, 6.0f, BeginDestroyProjectile, 7.0f, EndDestroyProjectile, 0, 2);

	}
	
}