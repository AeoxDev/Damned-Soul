#include "EntityFramework.h"
#include "Components.h"
#include "Model.h"
#include "UIComponents.h"
#include "Registry.h"
#include "Light.h"
#include "States\StateManager.h"

size_t Registry::GetEntityCount()
{
	//Since destroying an entity doesn't technically remove it (rather it stores its index into the availableEntitySlots-vector for future insertions) 
	//this is the "real" entity count. Don't think we need this I kinda just made it if we want to check to see how many "active" entities currently exist
	return entities.size() - availableEntitySlots.size();
}

EntityID Registry::CreateEntity(ENTITY_PERSISTENCY_TIER persistencyTier)
{
	//When we destroy an entity, we store its index and version in the freeEntities-vector so we know where we can create new entities later
	//If there's space in the freeEntities-vector, we use the version number stored in there. Otherwise we simply create a new id
	if (!availableEntitySlots.empty())
	{
		
		int newIndex = availableEntitySlots.back();
		availableEntitySlots.pop_back();

		EntityID newId = CreateEntityId(newIndex, false, persistencyTier); //GetEntityState(entities[newIndex].id)
		entities[newIndex].id = newId;
		entities[newIndex].persistencyTier = persistencyTier;
		return newId;
	}

	entities.push_back({ CreateEntityId((int)entities.size(), false, persistencyTier), EntityGlobals::componentBitset() });
	return entities.back().id;
}

void Registry::DestroyEntity(EntityID id)
{
	EntityID nullID = CreateEntityId(id.index, true, ENT_PERSIST_LOWEST);

	entities[GetEntityIndex(id)].id = nullID;
	entities[GetEntityIndex(id)].components.reset();

	availableEntitySlots.push_back(GetEntityIndex(id));
}

int compCount = 0;

void UnloadEntities(ENTITY_PERSISTENCY_TIER destructionTier)
{
	//Reset player specific variables that we know we want to reset between gamestates
	if (EntityGlobals::IsEntityValid(stateManager.player))
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
		if (player != nullptr && destructionTier >= 0) 
		{
			player->killingSpree = 0; 
			player->killThreshold = 0; 
			player->portalCreated = false;
		}
	}

	//Release memory from relevant components
	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		if (entity.persistentTier <= destructionTier)
		{
			ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
			ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		}
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
			ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		}	
	}

	for (auto entity : View<UIComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIComponent* ps = registry.GetComponent<UIComponent>(entity);
			ps->Release();
		}
	}

	for (auto entity : View<ProximityHitboxComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			ProximityHitboxComponent* p = registry.GetComponent<ProximityHitboxComponent>(entity);
			p->pointList.~ML_Vector();
		}
	}

	for (auto entity : View<TimedEventComponent>(registry))
	{
		if(destructionTier != ENT_PERSIST_PAUSE)
			ReleaseTimedEvents(entity);
	}

	if (destructionTier != ENT_PERSIST_PAUSE)
		Light::FreeLight();
	
	for (auto entity : View<SoundComponent>(registry))
	{
		if (destructionTier != ENT_PERSIST_PAUSE)
		{
			SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
			if (auto audioEngine = registry.GetComponent<AudioEngineComponent>(entity) == nullptr)
			{
				sound->Unload();
			}
		}
	}

	for (auto entity : View<AudioEngineComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);
			audioEngine->Destroy();
		}
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		//Get the current entity we're looking at in the registry
		EntityID check = registry.entities.at(i).id;

		//Destroy the entity if it isn't already destroyed and its persistency tier isn't greater than the destruction tier
		if (check.isDestroyed == false && check.persistentTier <= destructionTier)
		{
			registry.DestroyEntity(check);
		}
	}

	//Since UI can't depth-check, its entities need to be in numerical order, but availableEntitySlots is in ascending order and pops from the back
	registry.SortAvailableEntitySlotsVector();
}

void Registry::SortAvailableEntitySlotsVector()
{
	std::sort(availableEntitySlots.begin(), availableEntitySlots.end(), std::greater<int>());
}
