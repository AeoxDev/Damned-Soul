#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"

bool DamageOverTimeSystem::Update()
{
	// Won't update UI if the player is being DoT:ed

	for (auto entity : View<DamageOverTimeComponent, StatComponent>(registry))
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		DamageOverTimeComponent* DoT = registry.GetComponent<DamageOverTimeComponent>(entity);
		float dt = GetDeltaTime();
		float& remainingTime = DoT->remainingTime;
		// Check if the remaining time is larget than DT and get the shorter time
		float effectiveTime = dt < remainingTime ? dt : remainingTime;
		remainingTime -= dt;

		stats->health -= effectiveTime * DoT->damagePerSecond;

		if (remainingTime <= 0.f)
			registry.RemoveComponent<DamageOverTimeComponent>(entity);
	}

	return true;
}