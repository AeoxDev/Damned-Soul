#pragma once

#include "D3D11Graphics.h"

#include "Relics.h"

//Components:
/*
Graphics (d3d11 stuff, transform too)
Status (hp, movespeed, attack speed, damage)
Player (souls collected)
Enemy (possibly some bool checking to see if they're in an attack animation)
Weapon (move attack speed and damage variables here? depends how we want to do things)
StaticHazard (damage)
*/

//
struct GraphicsComponent
{
	/*sf::RectangleShape body;

	GraphicsComponent(int posX, int posY, int width, int height, sf::Color color)
	{
		body.setPosition((float)posX, (float)posY);
		body.setSize(sf::Vector2f((float)width, (float)height));
		body.setFillColor(color);
	}*/



};

//Temporary component for player
#define MAX_RELICS 8
struct PlayerComponent
{
#define MAX_LENGTH 16

	char name[MAX_LENGTH] = "Default Name";
	int hp = 100;

	std::bitset<MAX_RELICS> relicBitset; //I have become bitset enjoyer

	float damage = 20.0f;
	float defense = 0.0f; //percentage
	float speed = 0.25f;

	PlayerComponent(const char name_in[MAX_LENGTH], const int hp) :hp(hp)
	{
		std::memcpy(name, name_in, MAX_LENGTH);
	}

	struct RelicArray
	{
		RelicArray(size_t relicSize)
		{
			this->relicSize = relicSize;
			this->data = new char[relicSize];
		}

		inline void* get()
		{
			return data;
		}

		~RelicArray()
		{
			delete[] data;
		}

		char* data = nullptr;

	private:
		size_t relicSize = 0;
	};

	//Any time GetId() is called on a new type of relic, relicCount gets incremented, which results in the ID number for every component type being unique
	int relicCount = 0;
	template <typename T>
	int GetId()
	{
		static int id = relicCount++;
		return id;
	}

	template<typename T>
	void AddRelic()
	{
		//Ehe
		int id = GetId<T>();

		//Create relic and cast to RelicArray data
		T* relicPointer = new (relics->get()) T();

		//Set the component bitset of the entity at "id" to match the component we're passing in
		relicBitset.set(id);
	}

	template<typename T>
	T* GetRelic()
	{
		int id = GetId<T>(); //Since GetId is being called on a type T that it's been called on before, the id won't be incremented. It's brilliant honestly

		if (!relicBitset.test(id)) //Test to see if we have the relic before we try returning it
			return nullptr;

		//Get relic by casting RelicArray data back to the relic struct (but pointer)
		T* relicPointer = (T*)(relics->get());

		return relicPointer;
	}

	template<typename T>
	void RemoveRelic()
	{
		int id = GetId<T>();
		relicBitset.reset(id);
	}

private:
	RelicArray* relics = new RelicArray(sizeof(DamageRelic)); //Since all relics are the same size, this is fine
};










//DANGER ZONE
/*
struct StatusComponent
{
	int hp = 50;

	StatusComponent() = default;
	StatusComponent(int n) : hp(n) {}
};

struct EnemyComponent
{
#define MAX_LENGTH 16
	char name[MAX_LENGTH] = "Default Name";

	EnemyComponent() = default;
	EnemyComponent(const char s[MAX_LENGTH])
	{
		std::memcpy(name, s, MAX_LENGTH);
	}
};


struct GraphicsComponent
{
	//posX, posY, texture..
};

struct PositionComponent
{
	bool is3D;//false: position relative in screen, 0.0 to 1.0nb  
	float posX, posY, posZ;
	float lastPosX;
	float lastPosZ;
	float mass;
};

/// <summary>
/// This component is only for moving objects (e.g. walls do not need a physics component)
/// </summary>
*/