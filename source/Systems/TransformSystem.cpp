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

	//for (auto entity : View<TransformComponent, ParticleComponent>(registry))
	//{
	//	TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
	//	ParticleComponent* particles = registry.GetComponent<ParticleComponent>(entity);

	//	Particles::UpdateMetadata(particles->metadataSlot, tc->positionX, tc->positionY, tc->positionZ);
	//}
	return true;
}