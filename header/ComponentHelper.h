#pragma once
#include "Backend/Collision.h"

//Stats that every character in the game levels will have (player and enemies), modifyable by weapons and relics
struct StatComponent
{
private:
// Defensive stats
	//Base Max Health
	float m_baseHealth = 100.f;
	// Bonus Max Health, affected by things such as relics
	float m_bonusHealth = 0.f;
	// Current health
	float m_currentHealth = 100.f;
	
	// Multiplicative damage reduction, affected by things such as relics
	float m_damageReduction = 1.f;

// Movement stats
	// Base move speed
	float m_baseMoveSpeed = 1.f;
	// Bonus Move Speed, affected by things such as relics
	float m_bonusMoveSpeed = 0.f;
	// Used to alter speed when performing actions such as attacking
	float m_speedMult = 1.0f;


//Weapon stats
	// Damage
	float m_baseDamage = 10.f;
	// Bonus damage, affected by things such as relics
	float m_bonusDamage = 0.f;

	// Attack speed
	float m_baseAttackSpeed = 1.f;
	// Bonus attack speed, affected by things such as relics
	float m_bonusAttackSpeed = 0.f;

	// Knockback on hit
	float m_baseKnockback = 1.f;
	// Bonus knockback, affected by things such as relics
	float m_bonusKnockback = 0.f;
	// Used to alter knockback for certain special actions of the enemies
	float m_knockbackMultiplier = 1.f;

	// If the entity has bonus stats, used to skip entities in the system
	bool m_modifiedStats = false;
public:

	// for death animation
	bool performingDeathAnimation = false;

	//Hazards
	float baseHazardModifier = 1.0f;
	float hazardModifier = 1.0f;//Damage/slows and friction from hazards 0.0f or less means not affected.
	bool baseCanWalkOnCrack = false;//onCrack
	bool canWalkOnCrack = false;//If the entity can walk on cracks or not.

	StatComponent(float hp, float ms, float dmg, float as) : m_baseHealth(hp), m_currentHealth(hp), m_baseMoveSpeed(ms), m_baseDamage(dmg), m_baseAttackSpeed(as)
	{/* m_baseMoveSpeed = m_moveSpeed; */}

// Metadata
	// Mark the entity as being modified and having stat bonueses to calculate
	void MarkAsModified();
	// Check if the entity is modified and needs to have bonus stats calculated
	bool IsModified() const;
	// Set all stat bonuses to zero
	void ZeroBonusStats();
	

	

// Defensive Stats
	// Get max health
	float GetMaxHealth() const;
	// Get current health
	float GetHealth() const;
	// Get a value from 0 to 1 representing the current health of the entity
	float GetHealthFraction() const;
	// Update the entity's bonus health
	void UpdateBonusHealth(const float delta);
	// Update current health
	float UpdateHealth(const float delta, const bool hitByEnemy = false);

// Speed
	// Get the current speed
	float GetSpeed() const;
	// Update the entity's bonus speed
	void UpdateBonusSpeed(const float delta);
	// Set the entity's speed mult
	void SetSpeedMult(const float mult);

// Offensive
	// Get the damage of the entity
	float GetDamage() const;
	// update the entity's bonus damage
	void UpdateBonusDamage(const float delta);

	// Get the attack speed of the entity
	float GetAttackSpeed() const;
	// Update the entity's bonus attack speed
	void UpdateBonusAttackSpeed(const float delta);

	// Get the entity's knockback value
	float GetKnockback() const;
	// Update the entity's bonus knockback
	void UpdateBonusKnockback(const float delta);
	// Set the entity's knockback multiplier
	void SetKnockbackMultiplier(const float mult);
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
	EntityID lastPlayer = {-1, false, ENT_PERSIST_LOWEST};//The last player to hit the enemy
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