#include "Systems\Systems.h"
#include "Registry.h"
#include "Components.h"
#include "KnockBackComponent.h"
#include "DeltaTime.h"

bool KnockBackSystem::Update()
{
	float dt = GetDeltaTime();
	for (auto entity : View<KnockBackComponent, TransformComponent>(registry))
	{
		KnockBackComponent* kbc = registry.GetComponent<KnockBackComponent>(entity);
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);

		tc->positionX -= kbc->m_velocity_x * dt;
		tc->positionZ -= kbc->m_velocity_z * dt;

		kbc->Diminish(dt);
	}

	return true;
}