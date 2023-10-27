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
	//this is the "real" entity count
	return entities.size() - availableEntitySlots.size();
}

EntityID Registry::CreateEntity(int persistencyTier)
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
	EntityID nullID = CreateEntityId(-1, true, 0);

	entities[GetEntityIndex(id)].id = nullID;
	entities[GetEntityIndex(id)].components.reset();

	availableEntitySlots.push_back(GetEntityIndex(id));
}

int compCount = 0;
#define DESTROY_PLAYER 1
#define DESTROY_AUDIO 2
#define DESTROY_ALL 2
void UnloadEntities(int destructionTier)
{

	if (stateManager.player.index != -1)
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
		if (player != nullptr)
		{
			player->killingSpree = 0;
			player->killThreshold = 0;
			player->portalCreated = false;
		}
		
	}


	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{

		if (entity.persistentTier <= destructionTier)
		{
			ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
			ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
		}
		
	}

	for (auto entity : View<UIPlayerSoulsComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIPlayerSoulsComponent* ps = registry.GetComponent<UIPlayerSoulsComponent>(entity);
			ps->image.Release();
		}
	
	}

	/*for (auto entity : View<UIGameLevelComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIGameLevelComponent* ps = registry.GetComponent<UIGameLevelComponent>(entity);
			ps->image.Release();
		}
		
	}*/

	for (auto entity : View<UIPlayerRelicsComponent>(registry))
	{
		UIPlayerRelicsComponent* r = registry.GetComponent<UIPlayerRelicsComponent>(entity);
		r->baseImage.Release();

		for (uint32_t i = 0; i < r->relics.size(); i++)
		{
			r->relics[i].sprite.Release();
			r->relics[i].flavorTitleImage.Release();
			r->relics[i].flavorDescImage.Release();
		}

		r->relics.~ML_Vector();
	}

	for (auto entity : View<UIHealthComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIHealthComponent* ph = registry.GetComponent<UIHealthComponent>(entity);
			ph->backgroundImage.Release();
			ph->healthImage.Release();
		}
	}

	for (auto entity : View<UIButton>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIButton* b = registry.GetComponent<UIButton>(entity);
			b->Release();
		}
	}

	for (auto entity : View<UIShopComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIShopComponent* sh = registry.GetComponent<UIShopComponent>(entity);
			sh->baseImage.Release();
		}
	}

	for (auto entity : View<UIShopRelicWindowComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIShopRelicWindowComponent* sh = registry.GetComponent<UIShopRelicWindowComponent>(entity);
			sh->m_baseImage.Release();
		}
	}

	for (auto entity : View<UIRelicComponent>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIRelicComponent* sh = registry.GetComponent<UIRelicComponent>(entity);
			sh->sprite.Release();
			sh->flavorTitleImage.Release();
			sh->flavorDescImage.Release();
		}
	}

	for (auto entity : View<UIImage>(registry))
	{
		if (entity.persistentTier <= destructionTier)
		{
			UIImage* i = registry.GetComponent<UIImage>(entity);
			i->Release();
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
		EntityID check = registry.entities.at(i).id;
		if (check.index != -1)
		{
			////Make sure not to unload player or audio engine
			//PlayerComponent* skipPlayer = registry.GetComponent<PlayerComponent>(check);
			//AudioEngineComponent* skipAudioEngine = registry.GetComponent<AudioEngineComponent>(check);
			//if (destructionTier < DESTROY_PLAYER && (skipPlayer || skipAudioEngine))
			//{
			//	if (skipPlayer)
			//	{
			//		skipPlayer->killingSpree = 0;
			//		skipPlayer->portalCreated = false;
			//	}
			//	continue;
			//}
			//if (destructionTier < DESTROY_AUDIO && skipAudioEngine)
			//{
			//	
			//	continue;
			//}
			//if (check.state == false || destructionTier == DESTROY_ALL)
			//	

			if (check.persistentTier <= destructionTier)
				registry.DestroyEntity(check);

		}
	}
}