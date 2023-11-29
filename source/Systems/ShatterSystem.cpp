#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"

bool ShatterSystem::Update()
{
	for (auto entity : View<ShatterComponent>(registry))
	{
		auto comp = registry.GetComponent<ShatterComponent>(entity);
		comp->time += GetDeltaTime();
	}
}