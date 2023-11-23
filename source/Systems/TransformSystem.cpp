#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"
#include "Particles.h"
#include "DeltaTime.h"
#include "States\StateManager.h"

bool TransformSystem::Update()
{
	
	for (auto entity : View<TransformComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		tc->lastPositionX = tc->positionX;
		tc->lastPositionY = tc->positionY;
		tc->lastPositionZ = tc->positionZ;
		tc->positionX += tc->currentSpeedX * GetDeltaTime();
		tc->positionZ += tc->currentSpeedZ * GetDeltaTime();
	}

	//for (auto entity : View<TransformComponent, ParticleComponent>(registry))
	//{
	//	TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
	//	ParticleComponent* particles = registry.GetComponent<ParticleComponent>(entity);

	//	Particles::UpdateMetadata(particles->metadataSlot, tc->positionX, tc->positionY, tc->positionZ);
	//}
	return true;
}

void NormalizeFacing(TransformComponent* f)
{
	float len = sqrtf(f->facingX * f->facingX + f->facingZ * f->facingZ + f->facingY * f->facingY);
	f->facingX /= len;
	f->facingY /= len;
	f->facingZ /= len;
}

TransformComponent* GetPlayerTransform()
{
	if (stateManager.player.index != -1)
	{
		return registry.GetComponent<TransformComponent>(stateManager.player);
	}
	

	return nullptr;
}
