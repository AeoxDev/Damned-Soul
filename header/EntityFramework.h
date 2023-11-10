#pragma once
#include <vector>
#include <bitset>

#include "MemLib/MemLib.hpp"
#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_ComponentMap.hpp"
#include "EntityID.h"

/*
	//HOW TO USE (Basic version):

	//Step 1: Include the registry, this exists to keep track of all entities and whatever component(s) they should be associated with
	#include "Registry.h"

	//Step 2: Create an entity
	EntityID player = registry.CreateEntity();

	//Step 3: Create a component struct
	struct PlayerComponent
	{
		float someFloat = 1.0f;
		PlayerComponent(float x) : someFloat(x) {}
	}

	//Step 4: Add component(s) to the entity, pass in the arguments that the relevant component expects
	PlayerComponent* comp = registry.AddComponent<PlayerComponent>(player, 5.0f);

	//Step 5: Declare your own system in "Systems.h"
	class PlayerSystem() : public System
	{
		bool Update();
	}

	//Step 6: Define this system in a cpp file you create (in this case I could make a PlayerSystems.cpp)
	//Let it iterate over every entity in the registry containing the component(s) you specify and run some function (all system Update()-functions run every tick)
	#include "Systems\Systems.h"
	bool PlayerSystem::Update()
	{
		for(EntityID entity : View<PlayerComponent>(registry))
		{
			//Do what you want here. For example, you can get the component and then modify it in some way (probably depending on stuff happening in game)
			PlayerComponent* comp = registry.GetComponent<PlayerComponent>(entity);

			return true;
		}
	}

	//Step 7: Add this system to the vector of systems in the Setup()-function in "StateManager.cpp", pay attention to comments about how the order of systems matter
	systems.push_back(new PlayerSystem());


	//FUNCTIONS YOU'LL PROBABLY BE USING
	registry.CreateEntity()										-> Creates an entity, returns EntityID so it's easy to apply components
	registry.DestroyEntity(EntityID)							-> Destroys the entity with this ID

	registry.AddComponent<ComponentName>(EntityID, arguments)	-> Adds component to entity, returns the component if you want to make modifications to it right away
	registry.RemoveComponent<ComponentName>(EntityID)			-> Removes the specified component from entity
	registry.GetComponent<ComponentName>(EntityID)				-> Returns the specified component from entity

	for(EntityID entity : View<ComponentName>(registry))		-> Iterates through every entity with the specified component(s) in the registry

	If you're destroying entities for the sake of unloading in-between gamestates, make sure to release any memory that may need to be released
*/

//Global way of checking how many total components we've created
extern int compCount;

//Couple of global things needed specifically in here
namespace EntityGlobals
{
	//Define a bitset of all possible components (bitset is pretty much just a collection of on-off switches or booleans), currently upper limit is set to 64
	static constexpr int MAX_COMPONENTS = 64;
	typedef std::bitset<MAX_COMPONENTS> componentBitset;

	//Function to get the unique id of a component
	template <typename T>
	int GetId()
	{
		//Static variables are stored separately for each different template, so compCount only gets incremented whenever GetId() is called on a NEW type of component
		static int compId = compCount++;
		return compId;
	}

	static constexpr bool IsEntityValid(EntityID& id)
	{
		//Thinking if I should also make this check to see if the entity is currently considered to be "destroyed", but sticking with this for now
		return id.index != -1;
	}
}

//Main part of EntityFramework: the Registry, the collection of entities
class Registry
{
private:
	//From the user's perspective, EntityID is the only thing you'll care about
	//But this is the backend, so an entity is a little bit more than that
	struct Entity
	{
		//EntityID stores three things:
		//			1. Index in the registry
		//			2. Boolean state to check if the entity has been destroyed or not 
		//			3. Enum determining its persistency through the unloading of different gamestates
		EntityID id;

		EntityGlobals::componentBitset components;

		//Persistency scales from 0 to 3, where the tier is an arbitrary number referred to in the unloading of a gamestate
		//Whatever number is passed in when unloading, every entity with persistency EQUAL TO OR BELOW that number gets destroyed
		int persistencyTier = 0;

		//Quick operator hack giving better visualization to the user, you're welcome <3
		operator const int& () const { return id.index; }
	};

	//When we destroy entities, we add their indices to this vector to indicate where in the registry a new entity can be slotted into
	//We want to fill in the "empty" slots before pushing more entities into the back of the vector
	std::vector<int> availableEntitySlots;

public:
	std::vector<Entity> entities;

	//Returns the size of the entities-vector minus the size of the availableEntitySlots-vector to get the amount of "active" entities
	size_t GetEntityCount();

	//Creates an entity and returns its ID to the user for an easy way to start adding components to it right away
	EntityID CreateEntity(ENTITY_PERSISTENCY_TIER persistencyTier = ENT_PERSIST_LOWEST);

	//Destroys the entity with the specified ID
	void DestroyEntity(EntityID id, ENTITY_PERSISTENCY_TIER destructionTier = ENT_PERSIST_LOWEST);

	//Add a component T along with potential arguments to the entity with the specified ID
	template<typename T, typename ...Args>
	T* AddComponent(EntityID id, Args ...args)
	{
		//Don't try to access entity that has already been destroyed
		if (entities[GetEntityIndex(id)].id.isDestroyed)
			return nullptr;

		//Get the unique ID of the component we're trying to add
		int compId = EntityGlobals::GetId<T>();

		//If this ID number is greater than the size of the component map it's a new type component, so we make room for it
		if ((int)componentMaps.size() <= compId)
			componentMaps.resize(compId + 1); //Want to resize this to have a nullptr at the end but memlib nullptr be wildin

		//Initialize the component map at this component's ID, but only once
		if (false == componentMaps[compId].initialized())
			componentMaps[compId].Initialize(sizeof(T));

		//Create the component and slot T(args...) into the memory address at the specified index in the ComponentArray
		T* componentPointer = new (componentMaps[compId].emplace(id.index)) T(args...);

		//Set the component bitset at compId to true
		entities[GetEntityIndex(id)].components.set(compId);

		return componentPointer;
	}

	//Remove component T from entity at the specified ID
	template <typename T>
	void RemoveComponent(EntityID id)
	{
		//Don't try to access entity that has already been destroyed
		if (entities[GetEntityIndex(id)].id.isDestroyed)
			return;

		//Get the unique ID of the component we're trying to add
		int compId = EntityGlobals::GetId<T>();

		//Erase the component map at the specified ID and set its component bitset to false
		componentMaps[compId].erase(id.index);
		entities[GetEntityIndex(id)].components.reset(compId);
	}

	template<typename T>
	T* GetComponent(EntityID id)
	{
		//Don't try to access entity that has already been destroyed
		if (entities[GetEntityIndex(id)].id.isDestroyed)
			return nullptr;

		//Get the unique ID of the component we're trying to add
		int compId = EntityGlobals::GetId<T>();

		//Test to see if a component has been removed
		if (!entities[GetEntityIndex(id)].components.test(compId))
			return nullptr;

		//Get component by casting ComponentArray data (char*, address) back to the Component struct itself
		T* componentPointer = (T*)(componentMaps[compId][id.index]);

		return componentPointer;
	}

private:
	EntityID CreateEntityId(int index, bool hasBeenDestroyed, ENTITY_PERSISTENCY_TIER persistencyTier)
	{
		return { index, hasBeenDestroyed, persistencyTier };
	}

	int GetEntityIndex(EntityID id)
	{
		return id.index;
	}

	bool IsEntityDestroyed(EntityID id)
	{
		return id.isDestroyed;
	}

	void ReleaseComponentResources(EntityID id, ENTITY_PERSISTENCY_TIER destructionTier);

	//Private member variables
	std::vector<ML_ComponentMap> componentMaps;
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
			return entityIndex == other.entityIndex || entityIndex == (int)pRegistry->entities.size();
		}

		bool operator!=(const Iterator& other) const
		{
			return entityIndex != other.entityIndex && entityIndex != (int)pRegistry->entities.size();
		}

		bool ValidIndex() const
		{
			return EntityGlobals::IsEntityValid(pRegistry->entities[entityIndex].id) &&
				(components == (components & pRegistry->entities[entityIndex].components)); //Bitwise "and", checks to see that the things on either side of & are the same
		}

		Iterator& operator++()
		{
			//Iterate through the registry and increment the view's index but ONLY IF the entity has the appropriate component types and is a valid entity
			do
			{
				entityIndex++;
			} while (entityIndex < (int)pRegistry->entities.size() && !ValidIndex());
			return *this;
		}

		int entityIndex;
		Registry* pRegistry;
		EntityGlobals::componentBitset components;
	};

	const Iterator begin() const
	{
		int first = 0;
		while (first < (int)pRegistry->entities.size() &&
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