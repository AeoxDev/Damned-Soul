#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"
#include "Particles.h"

bool TransformSystem::Update()
{
	
	for (auto entity : View<TransformComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		tc->lastPositionX = tc->positionX;
		tc->lastPositionY = tc->positionY;
		tc->lastPositionZ = tc->positionZ;
	}
	for (auto entity : View<TransformComponent, LightComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		//LightComponent* light = registry.GetComponent<LightComponent>(entity);
		//Use the offset from light.
		OffsetPosition(entity, transform->positionX, transform->positionY, transform->positionZ, transform->facingX, transform->facingY, transform->facingZ);
		OffsetFacing(entity, transform->facingX, transform->facingY, transform->facingZ);
	}
	//for (auto entity : View<TransformComponent, ParticleComponent>(registry))
	//{
	//	TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
	//	ParticleComponent* particles = registry.GetComponent<ParticleComponent>(entity);

	//	Particles::UpdateMetadata(particles->metadataSlot, tc->positionX, tc->positionY, tc->positionZ);
	//}
	return true;
}