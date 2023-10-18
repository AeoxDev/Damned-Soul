#pragma once
#include <bitset>
#include "Relics.h"

//Stats that every character in the game levels will have (player and enemies), modifyable by weapons and relics
struct StatComponent
{
	//Base stats
	float health = 100.0f;
	//defense? percentage-based or flat?
	float moveSpeed = 1.0f;


	//Weapon stats
	float damage = 10.0f;
	float attackSpeed = 1.0f;


	// for death animation
	bool performingDeathAnimation = false;

	StatComponent(float hp, float ms, float dmg, float as) : health(hp), moveSpeed(ms), damage(dmg), attackSpeed(as) {}
};

//Stats specific to the player
struct PlayerComponent
{
	int souls = 0;
	int attackHitboxID = -1;
	int hardHitboxID = -1;
	int killingSpree = 0;
	bool portalCreated = false;
};

struct ControllerComponent
{
	float goalX = 0.0f, goalZ = -1.0f;//Goal direction

	float moveTime = .0f;//This is the time the player has been moving, used for camera feel.
	float moveFactor = 2.5f;
	float moveResetFactor = 1.25f;
	float moveMaxLimit = 3.0f;
};

//Ayaya
struct DashArgumentComponent
{
	//Exists to be able to be passed into functions without making the function arguments templated
	float x = 1.0f, z = 1.0f;
	float dashModifier = 2.0f;
	float arc = 0.0f; //Feliiiix

	DashArgumentComponent(float x, float z, float dashModifier, float arc = 0.0f) : x(x), z(z), dashModifier(dashModifier) {}
};

struct AttackArgumentComponent
{
	//Maybe I don't even need this lmao
	AttackArgumentComponent() {}
};

//
struct EnemyComponent
{
	int soulCount = 0;

	EnemyComponent(int sc) : soulCount(sc) {}
};

//
#define MAX_RELICS 8
struct RelicHolderComponent
{
#define MAX_LENGTH 16

	char name[MAX_LENGTH] = "Default Name";
	std::bitset<MAX_RELICS> relicBitset; //I have become bitset enjoyer

	RelicHolderComponent(const char name_in[MAX_LENGTH])
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