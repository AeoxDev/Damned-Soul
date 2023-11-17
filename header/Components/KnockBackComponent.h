#pragma once
#include "EntityID.h"
#define SELF_KNOCKBACK_FACTOR 0.2f


struct KnockBackComponent
{
	float m_velocity_x;
	float m_velocity_z;
	float friction;

	KnockBackComponent(const float x, const float z);

	void Diminish(const float deltaTime);
};

// Add Knock Back to an entity
void AddKnockBack(EntityID& entity, const float x, const float z);

// Calculate direction, not always needed
void CalculateKnockBackDirection(const EntityID& source, const EntityID& victim, float& x, float& z);

// Calculate velocity, not always needed
void CalculateVelocity(const EntityID& source, const EntityID& victim, float& v);

// Calculate Knock Back via entities
void CalculateKnockBack(const EntityID& source, const EntityID& victim, float& x, float& z);

// Calculate Knock Back via direction and velocity
void CalculateKnockBack(const float dx, const float dz, const float v, float& x, float& z);