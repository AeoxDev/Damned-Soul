#include "Relics\Utility\RelicParticleHelper.h"
#include "Registry.h"
#include "Components.h"

void _RelicParticleDespawn(EntityID& entity, const int& index)
{
	auto particles = registry.GetComponent<ParticleComponent>(entity);
	if (particles != nullptr)
	{
		particles->Release();
	}
	ReleaseTimedEvents(entity);
	registry.DestroyEntity(entity);
}

void ParticleAtEntityLocation(const EntityID& origin, const float duration, void* funcStart)
{
	EntityID percyJackson = registry.CreateEntity();
	TransformComponent percysLocation;
	TransformComponent* zeusLightning = registry.GetComponent<TransformComponent>(origin);

	// Move percy to his goal
	percysLocation.positionX = zeusLightning->positionX;
	percysLocation.positionY = zeusLightning->positionY;
	percysLocation.positionZ = zeusLightning->positionZ;
	registry.AddComponent<TransformComponent>(percyJackson, percysLocation);

	// Add a timed event for the particles
	AddTimedEventComponentStartEnd(percyJackson, 0, funcStart, duration, _RelicParticleDespawn);
}

void ParticleAtEntityFollow(const EntityID& origin, const float duration, void* funcStart)
{
	EntityID percyJackson = registry.CreateEntity();
	TransformComponent percysLocation;
	TransformComponent* zeusLightning = registry.GetComponent<TransformComponent>(origin);

	// Move percy to his goal
	percysLocation.positionX = zeusLightning->positionX;
	percysLocation.positionY = zeusLightning->positionY;
	percysLocation.positionZ = zeusLightning->positionZ;
	registry.AddComponent<TransformComponent>(percyJackson, percysLocation);
	registry.AddComponent<FollowerComponent>(percyJackson, origin);

	// Add a timed event for the particles
	AddTimedEventComponentStartEnd(percyJackson, 0, funcStart, duration, _RelicParticleDespawn);
}