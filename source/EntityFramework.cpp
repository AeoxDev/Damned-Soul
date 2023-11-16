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

void Registry::ReleaseComponentResources(EntityID id, ENTITY_PERSISTENCY_TIER destructionTier)
{
	LightComponent* light = registry.GetComponent<LightComponent>(id);
	if (light)
	{
		RemoveLight(id);
	}
	//Boneless model
	ModelBonelessComponent* boneless = registry.GetComponent<ModelBonelessComponent>(id);
	if (boneless)
		ReleaseModel(boneless->model);

	//Skeleton model
	ModelSkeletonComponent* skeleton = registry.GetComponent<ModelSkeletonComponent>(id);
	if (skeleton)
		ReleaseModel(skeleton->model);

	//OnClick
	OnClickComponent* onClick = registry.GetComponent<OnClickComponent>(id);
	if (onClick)
		onClick->Release();
	
	//OnHover
	OnHoverComponent* onHover = registry.GetComponent<OnHoverComponent>(id);
	if (onHover)
		onHover->Release();
	
	//UIComponent
	UIComponent* uiElement = registry.GetComponent<UIComponent>(id);
	if(uiElement)
		uiElement->Release();

	//Proximity Hitbox
	ProximityHitboxComponent* p = registry.GetComponent<ProximityHitboxComponent>(id);
	if(p)
		p->pointList.~ML_Vector();

	//TODO: Pass in persistency thing so we can check to see if it's NOT equal to ENT_PERSIST_PAUSE when unloading sound components
	if (destructionTier != ENT_PERSIST_PAUSE)
		ReleaseTimedEvents(id);
	
}

void Registry::DestroyEntity(EntityID id, ENTITY_PERSISTENCY_TIER destructionTier)
{
	EntityID nullID = CreateEntityId(id.index, true, ENT_PERSIST_LOWEST);

	//Check to see if the entity has any components that need to have d3d11 stuff released before destroying
	ReleaseComponentResources(id, destructionTier);

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

	if (destructionTier != ENT_PERSIST_PAUSE)
		Light::FreeLight();

	for (auto entity : View<TimedEventComponent>(registry))
	{
		if (destructionTier != ENT_PERSIST_PAUSE)
			ReleaseTimedEvents(entity);
	}

	for (auto entity : View<SoundComponent>(registry))
	{
		//Delete sound component from the pause buttons.
		if (destructionTier == ENT_PERSIST_PAUSE) //Shitty code, for a shitty scenario with the pause menu buttons. If any other entity during pause has an ONCLICK FKING COMPONENT THIS WILL BREAK AND I WILL BREAK YOU.
		{
			auto sound = registry.GetComponent<SoundComponent>(entity);
			auto button = registry.GetComponent<OnClickComponent>(entity);
			if ((sound != nullptr) && (button != nullptr))
			{
				sound->Unload();
			}
		}

		//Delete the sound component from every entity that is not the Player.
		if (destructionTier != ENT_PERSIST_PAUSE)
		{
			auto sound = registry.GetComponent<SoundComponent>(entity);
			if (registry.GetComponent<AudioEngineComponent>(entity) == nullptr && registry.GetComponent<PlayerComponent>(entity) == nullptr)
				sound->Unload();
		}

		//Delete the sound component from the player, but not if we are entering the shop or a new stage.
		if (entity.persistentTier <= destructionTier) //I hate how this code actually works
		{
			auto sound = registry.GetComponent<SoundComponent>(entity);
			if (registry.GetComponent<AudioEngineComponent>(entity) == nullptr && registry.GetComponent<PlayerComponent>(entity) != nullptr)
				sound->Unload();
		}
		//comments written by joaking
	}

	//If the audio engine is the last entity with a sound component, kill
	for (auto entity : View<AudioEngineComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			auto sound = registry.GetComponent<SoundComponent>(entity);
			if (sound)
				sound->Unload();
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
			registry.DestroyEntity(check, destructionTier);
		}
	}

	//Since UI can't depth-check, its entities need to be in numerical order, but availableEntitySlots is in ascending order and pops from the back
	registry.SortAvailableEntitySlotsVector();
}

void Registry::SortAvailableEntitySlotsVector()
{
	std::sort(availableEntitySlots.begin(), availableEntitySlots.end(), std::greater<int>());
}
