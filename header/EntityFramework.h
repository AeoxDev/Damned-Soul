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

//
struct ComponentPointer
{
	int id;			//What type of Component are we pointing to? StatusComponent, PlayerComponent, EnemyComponent, etc..
	void* address;	//Where is the Component struct located in memory?
	int size;		//What's the size of the Component struct?
};

template<typename T>
T& GetComponentValue(const ComponentPointer& comp)
{
	//Returns the actual specific Component stored at the address that the ComponentPointer is pointing at
	//Template magic courtesy of Elliot
	T* t = (T*)comp.address;

	//Dereference to get the value
	return *t;

	//Danger-zone
	//if (sizeof(T) == comp.size)
	//{
	//	//Convert void* to correct pointer type
	//	t = (T*)comp.address;

	//	//Dereference to get the value
	//	return *t;
	//}
	//else inout = T{0}; //Have some sort of error message here x
}

//All components go under here (hate the current "type" system, but I need some identifier letting us know what Component we're working with, which gets weird with templates)
struct StatusComponent
{
	int hp;

	StatusComponent() = default;
	StatusComponent(int n) : hp(n) {}
};

struct GraphicsComponent
{
	//posX, posY, texture..
};

struct PhysicsComponent
{
	float mass;
	float lastPosX;
	float lastPosY;
};

struct CircleColliderComponent
{
	float radius, offsetX, offsetY;
};

struct ConvexColliderComponent
{
	float centerX, centerY;

	int cornerAmount;
	float cornerX[16], cornerY[16];
};

struct GeometryIndependentColliderComponent
{
	uint8_t pixelMap[512 * 512]; //#define 512*512 as size? or rather just 512 as dimension
	float offsetX, offsetY;
};


struct PlayerComponent
{
	std::string name;

	PlayerComponent() = default;
	PlayerComponent(std::string s) : name(s) {}
};

template <typename T>
int GetComponentID()
{
	//int id = &(GetComponentID<T>);
	//Convert function to function pointer, convert that to void pointer, then take its value (adress) into an int
	return (int)((void*)&(GetComponentID<T>));
}

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

	std::vector<ComponentPointer> components;

	Entity(int idx, std::vector<ComponentPointer> vec) : index(idx), components(vec) {}

	template <typename T>
	bool HasComponent(T& comp)
	{
		for (const auto& component : components)
		{
			if (component.id == GetComponentID<T>())
				return true;
		}
		return false;
	}
};

struct View
{
	std::vector<Entity> entities;

	View(std::vector<Entity> in) : entities(in) {}

	size_t size() { return entities.size(); }
};

//Container for all our entities
struct Registry
{
	int index = 0;
	std::vector<Entity> entities;

	Registry() {}

	Entity CreateEntity()
	{
		std::vector<ComponentPointer> comps;

		Entity entity = Entity(index++, comps);
		entities.push_back(entity);

		return entity; //Return the entity as we create it so we can easily add components to this specific entity
	}

	size_t size() { return entities.size(); }

	template<typename T>
	void AddComponent(Entity& entity, T& toCreate)
	{
		//Convert any type of component (PlayerComponent, EnemyComponent, StatusComponent etc..) into the base Component struct (pointer tbh)
		ComponentPointer comp;
		comp.address = &toCreate;
		comp.size = sizeof(T);
		comp.id = GetComponentID<T>();

		//Add component to entity
		entity.components.push_back(comp);

		//Add to registry vector so we can iterate
		entities.at(entity.index).components.push_back(comp);
	}

	template <typename T, typename ...Args>
	View CreateView(Args ...args)
	{
		std::vector<Entity> toReturn;
		T comp;
		for (auto& entity : entities)
		{
			if (entity.HasComponent(comp))
			{
				toReturn.push_back(entity);
			}
		}
		return View(toReturn);
	}
};

/*
Currently we have to hard-code components, I want to change the way this works but for now: It is what it is
Example: StatusComponent playerHP(100);

Step 1: Create registry to hold all entities and their components	-->	Registry registry;
Step 2: Create entity												-->	auto entity = registry.CreateEntity();
Step 3: Add component(s) to the entity								-->	EntityLib::AddComponent(registry, entity, component);

If you want to get the value stored in this component (ehe)			-->	Iterate through registry.entities and do 
																		EntityLib::GetComponentValue<ComponentType>(entity.componentThatYouWantToGetValueOf)

If you want to get a view of only entities with specific components	-->	auto view = registry.CreateView<StatusComponent>()
*/