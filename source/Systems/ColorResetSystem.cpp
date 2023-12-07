#include "Registry.h"
#include "Components.h"
#include "Systems\Systems.h"

bool ColorResetSystem::Update()
{
	for (auto entity : View<ModelBonelessComponent>(registry))
		registry.GetComponent<ModelBonelessComponent>(entity)->shared.ResetTempColor();
	for (auto entity : View<ModelSkeletonComponent>(registry))
		registry.GetComponent<ModelSkeletonComponent>(entity)->shared.ResetTempColor();
	return true;
}