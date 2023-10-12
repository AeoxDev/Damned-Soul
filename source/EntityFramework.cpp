#include "EntityFramework.h"

size_t Registry::GetEntityCount()
{
	//Since destroying an entity doesn't technically remove it (rather it stores its index into the availableEntitySlots-vector for future insertions) 
	//this is the "real" entity count
	return entities.size() - availableEntitySlots.size();
}

EntityID Registry::CreateEntity()
{
	//When we destroy an entity, we store its index and version in the freeEntities-vector so we know where we can create new entities later
	//If there's space in the freeEntities-vector, we use the version number stored in there. Otherwise we simply create a new id
	if (!availableEntitySlots.empty())
	{
		int newIndex = availableEntitySlots.back();
		availableEntitySlots.pop_back();

		EntityID newId = CreateEntityId(newIndex, GetEntityState(entities[newIndex].id));
		entities[newIndex].id = newId;
		return newId;
	}

	entities.push_back({ CreateEntityId((int)entities.size(), false), EntityGlobals::componentBitset() });
	return entities.back().id;
}

void Registry::DestroyEntity(EntityID id)
{
	EntityID nullID = CreateEntityId(-1, true);

	entities[GetEntityIndex(id)].id = nullID;
	entities[GetEntityIndex(id)].components.reset();

	availableEntitySlots.push_back(GetEntityIndex(id));
}

int compCount = 0;