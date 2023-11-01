#pragma once
#include "Backend/Collision.h"

//Stats that every character in the game levels will have (player and enemies), modifyable by weapons and relics
struct StatComponent
{
private:
	//Base stats
	float maximumHealth = 100.f;
	float currentHealth = 100.f;
	//defense? percentage-based or flat?
public:
	float baseMoveSpeed = 1.0f;
	float moveSpeed = 1.0f;


	//Weapon stats
	float damage = 10.0f;
	float attackSpeed = 1.0f;

	float knockback = 1.0f;

	// for death animation
	bool performingDeathAnimation = false;

	//Hazards
	float hazardModifier = 1.0f;//Damage/slows and friction from hazards 0.0f or less means not affected.

	StatComponent(float hp, float ms, float dmg, float as) : maximumHealth(hp), currentHealth(hp), moveSpeed(ms), damage(dmg), attackSpeed(as) 
	{ baseMoveSpeed = moveSpeed; }

	// Get the current health of the player
	float GetHealth() const;
	// Get the max health of the player
	float GetMaxHealth() const;
	// Get a value from 0 to 1 representing the current health of the entity
	float GetHealthFraction() const;
	// Update the current health of the player
	float UpdateHealth(const float delta, const bool hitByEnemy = false);
	// Update the max health of the player
	float UpdateMaxHealth(const float delta);
};

//Stats specific to the player
struct PlayerComponent
{
private:
	// Set to private since it is important that any update is carried on through UpdateSouls
	int souls = 0;
public:
	int attackHitboxID = -1;
	int softHitboxID = -1;
	int killingSpree = 0;
	int killThreshold = 0;
	bool portalCreated = false;

	// Update the number of souls in the player's possession
	int UpdateSouls(const int delta);
	// Get the current number of souls the player possesses
	int GetSouls() const;
};

struct ControllerComponent
{
	float goalX = 0.0f, goalZ = -1.0f;//Goal direction

	float moveTime = .0f;//This is the time the player has been moving, used for camera feel.
	float moveFactor = 0.0f;
	float moveResetFactor = 0.0f;
	float moveMaxLimit = 0.0f;
	
	int enabled = 1;
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

struct CollisionParamsComponent
{
	OnCollisionParameters params;
	CollisionParamsComponent(OnCollisionParameters params) : params(params) {}
};

//
struct EnemyComponent
{
	int soulCount = 0;
	int attackHitBoxID = -1;
	int specialHitBoxID = -1;
	int type = -1;
	EnemyComponent(int sc, int t) : soulCount(sc), type(t) {}
};

//
//#define MAX_RELICS 8
//struct RelicHolderComponent
//{
//#define MAX_LENGTH 16
//
//	char name[MAX_LENGTH] = "Default Name";
//	std::bitset<MAX_RELICS> relicBitset; //I have become bitset enjoyer
//
//	RelicHolderComponent(const char name_in[MAX_LENGTH])
//	{
//		std::memcpy(name, name_in, MAX_LENGTH);
//	}
//
//	struct RelicArray
//	{
//		RelicArray(size_t relicSize)
//		{
//			this->relicSize = relicSize;
//			this->data = new char[relicSize];
//		}
//
//		inline void* get()
//		{
//			return data;
//		}
//
//		~RelicArray()
//		{
//			delete[] data;
//		}
//
//		char* data = nullptr;
//
//	private:
//		size_t relicSize = 0;
//	};
//
//	//Any time GetId() is called on a new type of relic, relicCount gets incremented, which results in the ID number for every component type being unique
//	int relicCount = 0;
//	template <typename T>
//	int GetId()
//	{
//		static int id = relicCount++;
//		return id;
//	}
//
//	template<typename T>
//	void AddRelic()
//	{
//		//Ehe
//		int id = GetId<T>();
//
//		//Create relic and cast to RelicArray data
//		T* relicPointer = new (relics->get()) T();
//
//		//Set the component bitset of the entity at "id" to match the component we're passing in
//		relicBitset.set(id);
//	}
//
//	template<typename T>
//	T* GetRelic()
//	{
//		int id = GetId<T>(); //Since GetId is being called on a type T that it's been called on before, the id won't be incremented. It's brilliant honestly
//
//		if (!relicBitset.test(id)) //Test to see if we have the relic before we try returning it
//			return nullptr;
//
//		//Get relic by casting RelicArray data back to the relic struct (but pointer)
//		T* relicPointer = (T*)(relics->get());
//
//		return relicPointer;
//	}
//
//	template<typename T>
//	void RemoveRelic()
//	{
//		int id = GetId<T>();
//		relicBitset.reset(id);
//	}
//
//private:
//	RelicArray* relics = new RelicArray(sizeof(DamageRelic)); //Since all relics are the same size, this is fine
//};