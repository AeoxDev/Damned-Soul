#pragma once
#include <iostream>
#include <vector>
#include <windows.h>

#include <bitset>
#include <Windows.h>

#include <map>

#include "MemLib/MemLib.hpp"
//#include "ComponentHelper.h"

/*
	//HOW TO USE (Basic version):

	//Step 1: Create registry, this exists to keep track of all entities and whatever component(s) they should be associated with
	Registry registry;

	//Step 2: Create an entity
	auto player = registry.CreateEntity();

	//Step 3: Add component(s) to the entity, pass in the arguments that the relevant Component expects
	registry.AddComponent<PlayerComponent>(player, "Mr Damned Soul");
	registry.AddComponent<StatusComponent>(player, 100);

	//And that's the absolute bare minimum of it, really. If you want to create a system for, let's say collision, you'd now do something like:
	auto enemy = registry.CreateEntity();
	registry.AddComponent<ColliderComponent>(enemy);
	for(auto entity : View<ColliderComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		//Check to see if this entity's hitbox overlaps with the player entity's hitbox, and then do stuff
	}

	//DOCUMENTATION

	//ENTITY FUNCTIONS
	registry.CreateEntity()										-> Creates an entity, returns EntityID so it's easy to apply components
	registry.DestroyEntity(EntityID)							-> Destroys the entity with this ID

	//COMPONENT FUNCTIONS
	registry.AddComponent<ComponentName>(EntityID, arguments)	-> Adds component to entity, type in whatever arguments the component constructor expects
	registry.RemoveComponent<ComponentName>(EntityID)			-> Removes the specified component from entity
	registry.GetComponent<ComponentName>(EntityID)				-> Returns the specified component from entity

	//VIEW
	View<ComponentName>(registry)								-> Creates a view that contains every entity with the specified component(s), can be iterated through
*/

//Just for better readability
struct EntityID
{
	int index;
	bool state; //Boolean to check if the entity has been destroyed or not
};

//Defines a componentBitset as a bitset made up of 32 bits, and function GetId (It's either this solution, or Registry and View redefines these)
namespace EntityGlobals
{
	static constexpr int MAX_COMPONENTS = 32;
	typedef std::bitset<MAX_COMPONENTS> componentBitset; //cppreference bitset: "N -> the number of bits to allocate storage for"

	static int compCount = 0;
	//Map
	static std::map<void*, int> componentOnBit;

	//Previously, GetId returned the current compCount and also incremented it, but let's split these
	template <typename T>
	int GetId()
	{
		
		//Note because of the increment that each time this function is called, the ID number will be new and unique
		static int compId = compCount++;
		int ret = componentOnBit[GetId<T>];
		return ret;
	}

	template <typename T>
	int CreateID()
	{

		//Note because of the increment that each time this function is called, the ID number will be new and unique
		static int compId = componentOnBit.size();
		componentOnBit.emplace(GetId<T>, componentOnBit.size());
		return compId;
	}

	static constexpr bool IsEntityValid(EntityID& id)
	{
		return id.index != -1;
	}

}

class Registry
{
private:
	struct Entity
	{
		EntityID id;
		EntityGlobals::componentBitset components;

		//Quick operator hack giving better visualization to the user, you're welcome <3
		operator const int& () const { return id.index; }
	};

	std::vector<int> availableEntitySlots;	//Contains the indices that are allowed to be used when creating new entitites (because they've for instance been 
											//destroyed prior, and we dont want to adjust the indices of existing entities)

public:
	std::vector<Entity> entities;

	size_t GetEntityCount()
	{
		//Since destroying an entity doesn't technically remove it (rather it stores its index into the availableEntitySlots-vector for future insertions) 
		//this is the "real" entity count
		return entities.size() - availableEntitySlots.size();
	}

	EntityID CreateEntity()
	{
		//When we destroy an entity, we store its index and version in the freeEntities-vector so we know where we can 
		//If there's space in the freeEntities-vector, we use the version number stored in there. Otherwise we simply create a new id with version number 0
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

	void DestroyEntity(EntityID id)
	{
		EntityID nullID = CreateEntityId(-1, true);

		entities[GetEntityIndex(id)].id = nullID;
		entities[GetEntityIndex(id)].components.reset();

		availableEntitySlots.push_back(GetEntityIndex(id));
	}

	template<typename T, typename ...Args>
	void AddComponent(EntityID id, Args ...args)
	{
		//Don't try to access entity that has already been removed
		if (entities[GetEntityIndex(id)].id.index != id.index)
			return;

		//Unique index of the component itself
		int compId = EntityGlobals::CreateID<T>();

		//Adding a new component that's greater than the size of the pool requires a resize for memory reasons
		if (componentArrays.size() <= compId) 
			componentArrays.resize(compId + 1);

		//New component, make a new array
		if (componentArrays[compId].IsNullptr())
		{
			//Allocate space in memory and call the "constructor" for the new array
			componentArrays[compId] = MemLib::palloc(sizeof(ComponentArray));
			componentArrays[compId]->Initialize(sizeof(T));
		}
		
		//PoolPointerPain to get create the actual component
		PoolPointer<T> typeCasted = componentArrays[compId]->data;
		typeCasted[GetEntityIndex(id)] = T(args...);

		//Set the component bitset of the entity at the current index to 
		entities[GetEntityIndex(id)].components.set(compId);
	}

	template <typename T>
	void RemoveComponent(EntityID id)
	{
		//Don't try to access entity that has already been removed
		if (entities[GetEntityIndex(id)].id.index != id.index)
			return;

		//Remove any component in this entity's component bitset that matches the component ID we're looking for
		int compId = EntityGlobals::GetId<T>();
		entities[GetEntityIndex(id)].components.reset(compId);
	}

	template<typename T>
	T* GetComponent(EntityID id)
	{
		//Don't try to access entity that has already been removed
		if (entities[GetEntityIndex(id)].id.index != id.index)
			return nullptr;

		int compId = EntityGlobals::GetId<T>();
		//Test to see if a component has been removed
		if (!entities[GetEntityIndex(id)].components.test(compId))
			return nullptr;

		//Return the component at the index of the specified entity ID
		PoolPointer<T> componentPointer = componentArrays[compId]->data;
		return &componentPointer[id.index];
	}

private:
	EntityID CreateEntityId(int index, bool hasBeenDestroyed)
	{
		return { index, hasBeenDestroyed };
	}

	int GetEntityIndex(EntityID id)
	{
		return id.index;
	}

	bool GetEntityState(EntityID id)
	{
		return id.state;
	}

	//Let's say only 10 entities can have the same component, just because we need to state something when we allocate memory
	static constexpr int MAX_ENTITIES = 10;
	struct ComponentArray
	{
		ComponentArray(size_t componentSize) //Allocate enough memory to hold as many entities as we allow, along with the size of components
		{
			this->componentSize = componentSize;
			this->data = MemLib::palloc(componentSize * MAX_ENTITIES);
		}

		void Initialize(const size_t& componentSize)
		{
			this->componentSize = componentSize;
			this->data = MemLib::palloc(componentSize * MAX_ENTITIES);
		}

		~ComponentArray()
		{
			MemLib::pfree(data);
		}

		PoolPointer<char> data;
		size_t componentSize = 0;
	};

	//Private member variables
	std::vector<PoolPointer<ComponentArray>> componentArrays;
};

//Templated to allow for views that can contain entities with any combination of components
template<typename ...Args>
class View
{
public:
	View(Registry& registry) : pRegistry(&registry)
	{
		//Array of integers for the id's ranging from 0 to however many component-types we're passing in
		int compIds[] = { 0, EntityGlobals::GetId<Args>()... };

		for (int i = 1; i < (sizeof...(Args) + 1); i++) //note that we start from 1, since compIds already holds a 0, everything AFTER that is actual component types
		{
			components.set(compIds[i]);
		}
	}

	struct Iterator
	{
		Iterator(Registry* pRegistry, int entityIndex, EntityGlobals::componentBitset components)
			: pRegistry(pRegistry), entityIndex(entityIndex), components(components) {}

		EntityID operator*() const
		{
			return pRegistry->entities[entityIndex].id;
		}

		bool operator==(const Iterator& other) const
		{
			return entityIndex == other.entityIndex || entityIndex == pRegistry->entities.size();
		}

		bool operator!=(const Iterator& other) const
		{
			return entityIndex != other.entityIndex && entityIndex != pRegistry->entities.size();
		}

		Iterator& operator++()
		{
			//Iterate through the registry and increment the view's index but ONLY IF the entity has the appropriate component types and is a valid entity
			do
			{
				entityIndex++;
			} while (entityIndex < pRegistry->entities.size() &&
				!EntityGlobals::IsEntityValid(pRegistry->entities[entityIndex].id) &&
				components != (components & pRegistry->entities[entityIndex].components)); //Bitwise "and", checks to see that the things on either side of & are the same, https://www.youtube.com/watch?v=HoQhw6_1NAA I can't believe he does it again		
			return *this;
		}

		int entityIndex;
		Registry* pRegistry;
		EntityGlobals::componentBitset components;
	};

	const Iterator begin() const
	{
		int first = 0;
		while (first < pRegistry->entities.size() &&
			(components != (components & pRegistry->entities[first].components) || !EntityGlobals::IsEntityValid(pRegistry->entities[first].id)))
			first++;
		return Iterator(pRegistry, first, components);
	}

	const Iterator end() const
	{
		return Iterator(pRegistry, (int)pRegistry->entities.size(), components);
	}

	Registry* pRegistry;
	EntityGlobals::componentBitset components;
};