#include "Systems/Systems.h"
#include "Registry.h"
#include "Components/Components.h"

bool FollowerSystem::Update()
{
	for (EntityID follower : View<FollowerComponent>(registry))
	{
		// Follow target's transform.
		FollowerComponent* comp = registry.GetComponent<FollowerComponent>(follower);
		TransformComponent* tComp = registry.GetComponent<TransformComponent>(follower);
		TransformComponent* target_tComp = registry.GetComponent<TransformComponent>(comp->m_target);
		if (tComp && target_tComp)
		{
			*tComp = *target_tComp;
		}

		AnimationComponent* aComp = registry.GetComponent<AnimationComponent>(follower);
		AnimationComponent* target_aComp = registry.GetComponent<AnimationComponent>(comp->m_target);
		if (aComp && target_aComp)
		{
			*aComp = *target_aComp;
		}
	}
	return true;
}