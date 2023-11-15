#include "KnockBackComponent.h"
#include "Registry.h"
#include "Components.h"

#define BASE_SPEED (10.f)
#define FRICTION_GROWTH_SPEED 5.0f

KnockBackComponent::KnockBackComponent(const float x, const float z)
{
	this->m_velocity_x = x * BASE_SPEED;
	this->m_velocity_z = z * BASE_SPEED;
	friction = 1.0f;
}

void KnockBackComponent::Diminish(const float deltaTime)
{
	// Calculate the sum of the components' size
	//float total = abs(m_velocity_x) + abs(m_velocity_z);
	
	// Calculate the movements
	/*float update_x = deltaTime * BASE_SPEED * m_velocity_x / total;
	float update_z = deltaTime * BASE_SPEED * m_velocity_z / total;*/

	//Elliot: Diminish like balloon?
	friction += deltaTime * FRICTION_GROWTH_SPEED;
	float dx = m_velocity_x * deltaTime * friction * friction;
	float dz = m_velocity_z * deltaTime * friction * friction;
	// Subtract or set to 0, whichever is more relevant
	//m_velocity_x = abs(update_x) < abs(m_velocity_x) ? m_velocity_x - update_x : 0;
	//m_velocity_z = abs(update_z) < abs(m_velocity_z) ? m_velocity_z - update_z : 0;
	if (abs(m_velocity_x) < abs(dx))
		m_velocity_x = 0.0f;
	else
		m_velocity_x -= dx;
	if (abs(m_velocity_z) < abs(dz))
		m_velocity_z = 0.0f;
	else
		m_velocity_z -= dz;
	//m_velocity_x -= dx;
	//m_velocity_z -= dy;
	//if (abs(m_velocity_x) < 0.01f)
	//{
	//	m_velocity_x = 0.0f;
	//}
	//if (abs(m_velocity_z) < 0.01f)
	//{
	//	m_velocity_z = 0.0f;
	//}
}

void AddKnockBack(EntityID& entity, const float x, const float z)
{
	KnockBackComponent* kbc = registry.GetComponent<KnockBackComponent>(entity);
	if (nullptr == kbc)
	{
		kbc = registry.AddComponent<KnockBackComponent>(entity, x, z);
		return;
	}

	kbc->m_velocity_x += x * BASE_SPEED;
	kbc->m_velocity_z += z * BASE_SPEED;
	kbc->friction = 1.0f;
}

void CalculateKnockBackDirection(const EntityID& source, const EntityID& victim, float& x, float& z)
{
	TransformComponent* tc1 = registry.GetComponent<TransformComponent>(source);
	TransformComponent* tc2 = registry.GetComponent<TransformComponent>(victim);

	// Differences should be exagurated, not diminished
	float delta_x = tc1->positionX - tc2->positionX;
	float delta_z = tc1->positionZ - tc2->positionZ;

	// Calutlate normalize length
	float len = powf(delta_x * delta_x + delta_z * delta_z, 0.5f);

	// Set "return" values
	x = delta_x / len;
	z = delta_z / len;
}

void CalculateVelocity(const EntityID& source, const EntityID& victim, float& v)
{
	TransformComponent* tc1 = registry.GetComponent<TransformComponent>(source);
	TransformComponent* tc2 = registry.GetComponent<TransformComponent>(victim);

	// Set "return" value
	v = tc1->mass / tc2->mass;
}

void CalculateKnockBack(const EntityID& source, const EntityID& victim, float& x, float& z)
{
	TransformComponent* tc1 = registry.GetComponent<TransformComponent>(source);
	TransformComponent* tc2 = registry.GetComponent<TransformComponent>(victim);

	// Differences should be exagurated, not diminished
	float delta_x = tc2->positionX - tc1->positionX;
	float delta_z = tc2->positionZ - tc1->positionZ;

	// Calutlate normalize length
	float len = powf(delta_x * delta_x + delta_z * delta_z, 0.5f);

	float multiplier = tc1->mass / tc2->mass;

	// Set "return" values
	x = multiplier * delta_x / len;
	z = multiplier * delta_z / len;
}

void CalculateKnockBack(const float dx, const float dz, const float v, float& x, float& z)
{
	x = dx * v;
	z = dz * v;
}
