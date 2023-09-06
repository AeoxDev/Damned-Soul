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

	StatusComponent(int n) : hp(n) {}
};

struct PlayerComponent
{
	std::string name;

	PlayerComponent(std::string s) : name(s) {}
};

struct EnemyComponent
{
	std::string name;

	EnemyComponent(std::string s) : name(s) {}
};
//end of: components

//Entity is just an index and some arbitrary amount of components
struct Entity
{
	std::pair<int, std::vector<Component>> container;

	Entity(std::pair<int, std::vector<Component>> in) { container = in; }
};

//Registry is just a vector of entities
struct Registry
{
	int idx = 0;
	std::vector<Entity> vec;

	Registry() {}
	Entity CreateEntity()
	{
		std::pair<int, std::vector<Component>> reg;
		reg.first = idx++;

		Entity entity = Entity(reg);
		vec.push_back(entity);
		return entity; //Return the entity as we create it so we can easily add components to this specific entity
	}

	void AddComponent(Entity& entity, const Component& comp)
	{
		vec.at(entity.container.first).container.second.push_back(comp); //cursed line
		entity.container.second.push_back(comp);
	}
};

template<typename T>
Component ConvertComponent(T& toConvert)
{
	//Convert any type of component (PlayerComponent, EnemyComponent, StatusComponent etc..) into the base Component struct
	Component comp;
	comp.address = &toConvert;
	comp.size = sizeof(T);
	return comp;
}

template<typename T>
void GetComponent(T& inout, const Component& comp)
{
	//Get the value stored at the address that the base Component struct is pointing at
	//Template compiler magic courtesy of Elliot
	T* t;
	if (sizeof(T) == comp.size)
	{
		//Convert void* to correct pointer type
		t = (T*)comp.address;

		//Dereference to get the value
		inout = *t;
	}
	//else inout = T{0}; //Have some sort of error message here x
}

//Relic of a bygone era
//int main()
//{
//	//Step 1: Create registry to hold all entities and their components
//	Registry registry;
//
//	//Step 1.5: Define whatever components you want
//	StatusComponent playerHP(420);
//	PlayerComponent playerName("Bruh");
//
//	StatusComponent enemyHP(69);
//	EnemyComponent enemyName("Cringe");
//
//	//Step 2: Create entity
//	auto player = registry.CreateEntity();
//
//	//Step 3: Add component(s)
//	registry.AddComponent(player, ConvertComponent(playerHP));
//	registry.AddComponent(player, ConvertComponent(playerName));
//
//	//Repeat step 2 and 3 for arbitrary amount of entities
//	auto enemy = registry.CreateEntity();
//
//	registry.AddComponent(enemy, ConvertComponent(enemyHP));
//	registry.AddComponent(enemy, ConvertComponent(enemyName));
//
//	//Step 4: Cry
//	for (auto& entity : registry.vec)
//	{
//		std::cout << "Entity #" << entity.container.first << std::endl;
//		//Get back component
//		StatusComponent getMeBack = StatusComponent(0);
//		GetComponent(getMeBack, entity.container.second.at(0));
//		std::cout << "HP: " << getMeBack.hp << std::endl;
//		PlayerComponent getMeBackToo = PlayerComponent("");
//		GetComponent(getMeBackToo, entity.container.second.at(1));
//		std::cout << "Name: " << getMeBackToo.name << std::endl;
//
//		std::cout << std::endl;
//	}
//
//	return 0;
//}