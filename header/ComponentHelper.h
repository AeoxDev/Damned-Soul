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

	// Dash
	// Base dash value
	float m_baseDashValue = 2.5f;
	// Bonus dash value
	float m_bonusDashValue = 0.f;



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
	float m_acceleration = 1.0f;//How much of speed to gain over one second
	float m_baseAcceleration = 1.0f;//How much of speed to gain over one second
	// for death animation
	bool performingDeathAnimation = false;

	//Hazards
	float baseHazardModifier = 1.0f;
	float hazardModifier = 1.0f;//Damage/slows and friction from hazards 0.0f or less means not affected.
	bool baseCanWalkOnCrack = false;//onCrack
	bool canWalkOnCrack = false;//If the entity can walk on cracks or not.
	float iceAccelFactor = 0.08f;
	float iceAnimFactor = 2.5f;
	float lavaAccelFactor = 0.5f;
	float lavaAnimFactor = 2.0f;
	float acidAccelFactor = 0.3f;
	float acidAnimFactor = 0.5f;


	StatComponent(float hp, float ms, float dmg, float as) : m_baseHealth(hp), m_currentHealth(hp), m_baseMoveSpeed(ms), m_baseDamage(dmg), m_baseAttackSpeed(as)
	{/* m_baseMoveSpeed = m_moveSpeed; */
		m_baseAcceleration = ms;
		m_acceleration = ms;
	}

// Metadata
	// Mark the entity as being modified and having stat bonueses to calculate
	void MarkAsModified();
	// Check if the entity is modified and needs to have bonus stats calculated
	bool IsModified() const;
	// Set all stat bonuses to zero
	void ZeroBonusStats();
	

	

// Defensive Stats
	// Get max health
	int64_t GetMaxHealth() const;
	// Get current health
	int64_t GetHealth() const;
	// Get a value from 0 to 1 representing the current health of the entity
	float GetHealthFraction() const;
	// Update the entity's bonus health
	void UpdateBonusHealth(const float delta);

	// Limit current health to max health
	float CapHealth();
	// Update current health while not triggering triggered effects (aside from updating UI)
	// This should NOT normally be called anywhere without very good reason, use ApplyDamage or ApplyHealing instead
	// Negative values becomes damage, positive values becomes healing
	float StealthilyModifyHealth(const float delta);
	// Apply damage
	float ApplyDamage(const float damage, const bool hitByEnemy = false);
	// Apply healing
	float ApplyHealing(const float healing, const bool hitByEnemy = false);

// Speed
	// Get the current speed
	float GetSpeed() const;
	// Update the entity's bonus speed
	void UpdateBonusSpeed(const float delta);
	// Set the entity's speed mult
	void SetSpeedMult(const float mult);
	
	// Get the current dash distance
	float GetDashDistance() const;
	// Update Bonus Dash
	void UpdateBonusDashDistance(const float delta);

// Offensive
	// Get the base damage of the entity
	float GetBaseDamage() const;
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
	int dashHitboxID = -1;
	int killingSpree = 0;
	int killThreshold = 0;
	bool portalCreated = false;

	//New additions because of player attack chains
	float timeSinceLastAttack = -1.0f;
	unsigned int attackChainIndex = 0;
	bool isAttacking = false;
	bool hasActivatedHitbox = false;

	//New additions because of player heavy attacks
	float currentCharge = 0.0f;
	float maxCharge = 1.0f; 

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

struct AttackArgumentComponent
{
	float duration = 0.0f;
};

struct ChargeAttackArgumentComponent
{
	float multiplier = 0.0f;
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