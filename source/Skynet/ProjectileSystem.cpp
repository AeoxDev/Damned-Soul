#include "Systems/Systems.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"

bool ProjectileSystem::Update()
{
	for (auto bullet : View<ProjectileComponent, TransformComponent, StatComponent, HitboxComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(bullet);
		StatComponent* stats = registry.GetComponent<StatComponent>(bullet);
		transform->positionX += stats->GetSpeed() * transform->facingX * GetDeltaTime();
		transform->positionZ += stats->GetSpeed() * transform->facingZ * GetDeltaTime();
	}

	return true;
}