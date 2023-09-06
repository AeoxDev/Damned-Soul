#pragma once

#include <vector>
#include <string>

/*
Entity itself is essentially just an indicator/index. Start at 0 and it's just that shrimple :^)
The shmeat of the whole thing is a list of every entity and every component associated with it (called Registry in EnTT)

Functions that will be needed for this registry/list:
Create() just creates it
AddEntity() adds an entity at index (starting at 0, then increment)
AddComponent(), templated function that adds some component (struct with some data) to the current Entity (or do we want some sort of way of choosing which Entity to add component to?)
Do we want this registry to be an unordered map? Since technically the "Entity" indicator itself could be the "key"? Or maybe that'll brick itself when we start removing entities from the list?
*/

//Base Component struct
struct Component
{
	int size;
	void* address;
};

//All components go under here
struct StatusComponent
{
	int hp;

	StatusComponent() = default;
	StatusComponent(int n) : hp(n) {}
};

struct PlayerComponent
{
	std::string name;

	PlayerComponent() = default;
	PlayerComponent(std::string s) : name(s) {}
};

struct EnemyComponent
{
	std::string name;

	EnemyComponent() = default;
	EnemyComponent(std::string s) : name(s) {}
};
//end of: components

//Entity is just an index and some arbitrary amount of components
struct Entity
{
	int index;
	std::vector<Component> components;

	Entity(int idx, std::vector<Component> vec) : index(idx), components(vec) {}
};

//Container for all our entities
struct Registry
{
	int index = 0;
	std::vector<Entity> entities;

	Registry() {}

	Entity CreateEntity()
	{
		std::vector<Component> comps;

		Entity entity = Entity(index++, comps);
		entities.push_back(entity);

		return entity; //Return the entity as we create it so we can easily add components to this specific entity
	}
};

//Encapsulate to be more clear if we use it across several text-files? idk might be unnecessary
namespace EntityLib
{
	template<typename T>
	void AddComponent(Registry& registry, Entity& entity, T& toCreate)
	{
		//Convert any type of component (PlayerComponent, EnemyComponent, StatusComponent etc..) into the base Component struct (pointer tbh)
		Component comp;
		comp.address = &toCreate;
		comp.size = sizeof(T);

		//Add component to entity
		entity.components.push_back(comp);

		//Add to registry vector so we can iterate
		registry.entities.at(entity.index).components.push_back(comp);
	}

	template<typename T>
	T& GetComponentValue(const Component& comp)
	{
		//Get the value stored at the address that the base Component struct is pointing at
		//Template compiler magic courtesy of Elliot
		T* t;
		if (sizeof(T) == comp.size)
		{
			//Convert void* to correct pointer type
			t = (T*)comp.address;

			//Dereference to get the value
			return *t;
		}
		//else inout = T{0}; //Have some sort of error message here x
	}
}

/*
Currently we have to hard-code components, I want to change the way this works but for now: It is what it is
Example: StatusComponent playerHP(100);

Step 1: Create registry to hold all entities and their components	-->	Registry registry;
Step 2: Create entity												-->	auto entity = registry.CreateEntity();
Step 3: Add component(s) to the entity								-->	EntityLib::AddComponent(registry, entity, component);

If you want to get the value stored in this component (ehe)			-->	Iterate through registry.entities and do 
																		EntityLib::GetComponentValue<ComponentType>(entity.componentThatYouWantToGetValueOf)
*/