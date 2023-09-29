#pragma once
#include <iostream>
#include <vector>
#include <windows.h>

#include <bitset>
#include <Windows.h>

#include <map>

#include "MemLib/MemLib.hpp"
#include "MemLib/ML_Vector.hpp"
#include "ComponentHelper.h"

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

extern int compCount;
namespace EntityGlobals
{
	static constexpr int MAX_COMPONENTS = 32;
	typedef std::bitset<MAX_COMPONENTS> componentBitset; //cppreference bitset: "N -> the number of bits to allocate storage for"


	//compCount only gets incremented whenever GetId() is called on a NEW type of component
	template <typename T>
	int GetId()
	{
		//Note because of the increment that each time this function is called, the ID number will be new and unique
		static int compId = compCount++;
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

	void DestroyEntity(EntityID id)
	{
		EntityID nullID = CreateEntityId(-1, true);

		entities[GetEntityIndex(id)].id = nullID;
		entities[GetEntityIndex(id)].components.reset();

		availableEntitySlots.push_back(GetEntityIndex(id));
	}

	//template<typename T, typename ...Args>
	//void AddComponent(EntityID id, Args ...args)
	//{
	//	//Don't try to access entity that has already been removed
	//	if (entities[GetEntityIndex(id)].id.index != id.index)
	//		return;

	//	//Unique index of the component itself
	//	int compId = EntityGlobals::GetId<T>();

	//	//Adding a new component that's greater than the size of the pool requires a resize for memory reasons
	//	//So we add +1 to the size of the array and set the value at this new spot to nullptr (NOT DOING THIS RN BECAUSE POOLPOINTER CAN'T BE STRAIGHT-UP SET TO NULLPTR, ASK HERMAN)
	//	//if ((int)componentArrays.size() <= compId) 
	//	//	componentArrays.resize(compId + 1);

	//	//New component, make a new array, NORMALLY: componentArrays[compId] = new ComponentArray(sizeof(T))
	//	if (componentArrays[compId].IsNullptr())
	//	{
	//		//Allocate space in memory and call the "constructor" for the new array
	//		componentArrays[compId] = MemLib::palloc(sizeof(ComponentArray));
	//		componentArrays[compId]->Initialize(sizeof(T));
	//	}
	//	
	//	//PoolPointerPain to get create the actual component
	//	//Create component and cast to ComponentArray data (char*, address), NORMALLY: T* componentPointer = new (componentArrays[compId]->data) T(args...);
	//	PoolPointer<T> typeCasted = componentArrays[compId]->data;
	//	typeCasted[GetEntityIndex(id)] = T(args...);

	//	//Set the component bitset
	//	entities[GetEntityIndex(id)].components.set(compId);
	//}

	template<typename T, typename ...Args>
	T* AddComponent(EntityID id, Args ...args)
	{
		//Don't try to access entity that has already been removed
		if (entities[GetEntityIndex(id)].id.index != id.index)
			return nullptr;

		//Unique index of the component itself
		int compId = EntityGlobals::GetId<T>();

		//Adding a new component that's greater than the size of the pool requires a resize for memory reasons
		if ((int)componentArrays.size() <= compId) 
			componentArrays.resize(compId + 1); //Want to resize this to have a nullptr at the end but memlib nullptr be wildin

		//New component, make a new array
		if (componentArrays[compId].IsNullptr())
		{
			//Allocate space in memory and call the "constructor" for the new array
			//componentArrays[compId] = new ComponentArray(sizeof(T));
			//componentArrays[compId] = 
			componentArrays[compId] = MemLib::palloc(sizeof(ComponentArray));
			new(componentArrays[compId]) ComponentArray(sizeof(T));
			//componentArrays[compId]->Initialize(sizeof(T));
		}

		//Create component and cast to ComponentArray data (char*, address)
		//Edit: Not quite true, more like we're slotting T(args...) into the memory address at the specified index in the ComponentArray
		T* componentPointer = new (componentArrays[compId]->GetDataAt(id)) T(args...);
		//PoolPointer<T> typeCasted = (componentArrays[compId]->GetDataAt(id)) T(args...);
		//typeCasted[GetEntityIndex(id)] = T(args...);

		//Set the component bitset
		entities[GetEntityIndex(id)].components.set(compId);

		return componentPointer;
		//return &componentPointer;
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
		//Get component by casting ComponentArray data (char*, address) back to the Component struct itself, NORMALLY: T* componentPointer = (T*)(componentArrays[compId]->data);
		T* componentPointer = (T*)(componentArrays[compId]->GetDataAt(id));
		//PoolPointer<T> componentPointer = componentArrays[compId]->GetDataAt(id);

		//return &componentPointer;
		return componentPointer;
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
			//this->data = new char[componentSize * MAX_ENTITIES];
			this->data = MemLib::palloc(componentSize * MAX_ENTITIES);
		}

		void Initialize(const size_t& componentSize)
		{
			this->componentSize = componentSize;
			this->data = MemLib::palloc(componentSize * MAX_ENTITIES);
		}

		inline void* GetDataAt(EntityID id)
		{
			return data + id.index * componentSize;
		}

		~ComponentArray()
		{
			//delete[] data;
			MemLib::pfree(data);
		}

		PoolPointer<char> data;
		//char* data;
		size_t componentSize = 0;
	};

	//Private member variables
	//ML_Vector<PoolPointer<ComponentArray>> componentArrays;
	//std::vector<ComponentArray*> componentArrays;
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
			} while (entityIndex < (int)pRegistry->entities.size() && !ValidIndex()); //Way cleaner ngl
			//Previously:
			/*while (entityIndex < (int)pRegistry->entities.size() &&
				!(EntityGlobals::IsEntityValid(pRegistry->entities[entityIndex].id) &&
					(components == (components & pRegistry->entities[entityIndex].components))));*/
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


//System implementation that'll pretty much need to be worked into the scene class and not fuck up the rest of the project
//Order of operations in the main update loop is essentially:
//1. Do general window events
//2. Clear
//3. Update systems (includes Draw-calls)
//4. Display
//And repeat
/*
//Funny system update loop
for (auto it = systems.begin(); it != systems.end(); it++)
{
	if ((*it)->Update(registry, dt))
	{
		//Update does whatever it does, and IF it returns true afterwards we delete the system
		delete (*it);
		it = systems.erase(it);
		if (systems.size() == 0) //No more update if we've officially run out of systems to update
			return;
	}
}

#pragma once

#include "EntityFramework.h"

//Global window ehe
sf::String defaultTitle = "Active Relic: ";
sf::RenderWindow window(sf::VideoMode(800, 600), defaultTitle);

class System
{
public:
	virtual bool Update(Registry& registry, float dt) = 0;
};

class PlayerSystem : public System
{
public:
	bool Update(Registry& registry, float dt)
	{
		auto view = View<PlayerComponent, GraphicsComponent>(registry);
		for (auto entity : view)
		{
			auto playerComp = registry.GetComponent<PlayerComponent>(entity);
			auto graphicsComp = registry.GetComponent<GraphicsComponent>(entity);

			Move(playerComp, graphicsComp, dt);
		}

		return false;
	}

private:
	void Move(PlayerComponent* playerComp, GraphicsComponent* graphicsComp, float dt)
	{
		float speed = playerComp->speed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			playerComp->AddRelic<SpeedRelic>();
			window.setTitle(defaultTitle + "Speed");
		}
		else
		{
			playerComp->RemoveRelic<SpeedRelic>();
			window.setTitle(defaultTitle + "None");
		}

		if (playerComp->GetRelic<SpeedRelic>()) //If the player has the speed relic they move at double the speed
			speed *= 2.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			graphicsComp->body.move(speed * dt, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			graphicsComp->body.move(-speed * dt, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			graphicsComp->body.move(0.0f, -speed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			graphicsComp->body.move(0.0f, speed * dt);
		}
	}
};

class GraphicsSystem : public System
{
public:
	GraphicsSystem(sf::RenderWindow* window) : window(window) {}

	bool Update(Registry& registry, float dt)
	{
		auto view = View<GraphicsComponent>(registry);
		for (auto entity : view)
		{
			auto comp = registry.GetComponent<GraphicsComponent>(entity);
			window->draw(comp->body);
		}

		return false;
	}
private:
	sf::RenderWindow* window;
};
*/