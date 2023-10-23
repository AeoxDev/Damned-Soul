#include "EntityFramework.h"
#include "Components.h"
#include "Model.h"
#include "UIComponents.h"
#include "Registry.h"
#include "Light.h"

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

		EntityID newId = CreateEntityId(newIndex, false); //GetEntityState(entities[newIndex].id)
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

void UnloadEntities(bool unloadPersistents)
{
	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<UIPlayerSoulsComponent>(registry))
	{
		UIPlayerSoulsComponent* ps = registry.GetComponent<UIPlayerSoulsComponent>(entity);
		ps->image.Release();
	}

	for (auto entity : View<UIGameLevelComponent>(registry))
	{
		UIGameLevelComponent* ps = registry.GetComponent<UIGameLevelComponent>(entity);
		ps->image.Release();
	}

	for (auto entity : View<UIPlayerRelicsComponent>(registry))
	{
		UIPlayerRelicsComponent* r = registry.GetComponent<UIPlayerRelicsComponent>(entity);
		r->baseImage.Release();

		for (uint32_t i = 0; i < r->relics.size(); i++)
		{
			r->relics[i].sprite.Release();
			r->relics[i].flavorImage.Release();
		}

		r->relics.~ML_Vector();
	}

	for (auto entity : View<UIHealthComponent>(registry))
	{
		UIHealthComponent* ph = registry.GetComponent<UIHealthComponent>(entity);
		ph->backgroundImage.Release();
		ph->healthImage.Release();
	}

	for (auto entity : View<UIButton>(registry))
	{
		UIButton* b = registry.GetComponent<UIButton>(entity);
		b->Release();
	}

	for (auto entity : View<UIImage>(registry))
	{
		UIImage* i = registry.GetComponent<UIImage>(entity);
		i->Release();
	}

	for (auto entity : View<ProximityHitboxComponent>(registry))
	{
		ProximityHitboxComponent* p = registry.GetComponent<ProximityHitboxComponent>(entity);
		p->pointList.~ML_Vector();
	}

	for (auto entity : View<TimedEventComponent>(registry))
	{
		ReleaseTimedEvents(entity);
	}

	Light::FreeLight();
	for (auto entity : View<SoundComponent>(registry))
	{
		SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
		if (auto audioEngine = registry.GetComponent<AudioEngineComponent>(entity) == nullptr)
		{
			sound->Unload();
		}
	}

	if (unloadPersistents)
	{
		for (auto entity : View<AudioEngineComponent>(registry))
		{
			AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);
			audioEngine->Destroy();
		}
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		EntityID check = registry.entities.at(i).id;
		if (check.index != -1)
		{
			if (auto comp = registry.GetComponent<AudioEngineComponent>(check) != nullptr)
			{
				if ((check.state == false && !comp) || unloadPersistents)
					registry.DestroyEntity(check);
			}
			else if (check.state == false || unloadPersistents)
				registry.DestroyEntity(check);
		}
	}

	//Destroy entity resets component bitmasks
	//for (int i = 0; i < registry.entities.size(); i++)
	//{
	//	EntityID check = registry.entities.at(i).id;
	//	/*if (!unloadPersistents && registry.GetComponent<PlayerComponent>(check) != nullptr)
	//	{
	//		continue;
	//	}*/
	//	if (check.state == false)
	//		registry.DestroyEntity(check);
	//}
}