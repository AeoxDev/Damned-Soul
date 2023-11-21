#pragma once
#include "EntityFramework.h"
#include "Backend/Collision.h"

//Format
//void(*CollisionFunction)( , EntityID& entity1,
//  EntityID& entity2, const float& normal1X, const  float& normal1Z,
//  const  float& normal2X, const  float& normal2Z);

void NoCollision(OnCollisionParameters& params);

void SoftCollision(OnCollisionParameters& params);
void HardCollision(OnCollisionParameters& params);

//Damage Collisions
bool IsDamageCollisionValid(OnCollisionParameters& params);
void DashCollision(OnCollisionParameters& params);
void AttackCollision(OnCollisionParameters& params);
void ShockWaveAttackCollision(OnCollisionParameters& params);
void ProjectileAttackCollision(OnCollisionParameters& params);
void HellhoundBreathAttackCollision(OnCollisionParameters& params);
void HazardAttackCollision(OnCollisionParameters& params);

void LoadNextLevel(OnCollisionParameters& params);
