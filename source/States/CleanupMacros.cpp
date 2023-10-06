#include "States\CleanupMacros.h"

uint16_t DestroyEntities(const ML_Map<int, EntityID>& entities)
{
	uint16_t retVal = 0;
	for (auto& [index, entity] : entities)
	{
		++retVal;
		registry.DestroyEntity(entity);
	}
	return retVal;
};