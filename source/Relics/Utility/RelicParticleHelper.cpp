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
	EntityID mortalPercy = registry.CreateEntity();
	TransformComponent percysLocation;
	TransformComponent* zeusLightning = registry.GetComponent<TransformComponent>(origin);
	registry.AddComponent<FollowerComponent>(mortalPercy, origin);

	// Move percy to his goal
	percysLocation.positionX = zeusLightning->positionX;
	percysLocation.positionY = zeusLightning->positionY;
	percysLocation.positionZ = zeusLightning->positionZ;
	registry.AddComponent<TransformComponent>(mortalPercy, percysLocation);
																//Just a control number, can be anything
	registry.AddComponent<ParentComponent>(mortalPercy, origin, 1337);
	registry.AddComponent<ParentControlComponent>(origin, 1337);

	// Add a timed event for the particles
	AddTimedEventComponentStartContinuousEnd(mortalPercy, 0, funcStart, _CheckLiving, duration, _RelicParticleDespawn);
}

void _CheckLiving(const EntityID& child, const int& index)
{
	ParentComponent* parent = registry.GetComponent<ParentComponent>(child);
	ParentControlComponent* parentCon = registry.GetComponent<ParentControlComponent>(parent->parentID);

	if (parentCon == nullptr)
	{
		//Parent is dead, kill the child
		auto particles = registry.GetComponent<ParticleComponent>(child);
		if (particles != nullptr)
		{
			particles->Release();
		}
		ReleaseTimedEvents(child);
		registry.DestroyEntity(child);
	}
	else
	{
		for (auto pEntity : View<ParentControlComponent>(registry))
		{
			if (pEntity.isDestroyed)
			{
				auto particles = registry.GetComponent<ParticleComponent>(child);
				if (particles != nullptr)
				{
					particles->Release();
				}
				ReleaseTimedEvents(child);
				registry.DestroyEntity(child);
			}
		}
	}
}
