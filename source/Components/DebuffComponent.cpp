#include "DebuffComponent.h"
#include "DeltaTime.h"

DamageOverTime::DamageOverTime(const float DpS, const float time)
{
	damagePerSecond = DpS;
	remainingTime = time;
}

bool DamageOverTime::LessThan(const DamageOverTime& other)
{
	return (remainingTime * damagePerSecond) < (other.remainingTime * other.damagePerSecond);
}

float DamageOverTime::GetTime() const
{
	float dt = GetDeltaTime();
	return dt < remainingTime ? dt : remainingTime;
}

float DamageOverTime::GetDPS() const
{
	return damagePerSecond;
}

bool DamageOverTime::Advance(const float dt)
{
	return (remainingTime -= dt) <= 0;
}

DamageOverTime::operator bool()
{
	return (0 < remainingTime) && (0 < damagePerSecond);
}

Frozen::Frozen(const float time)
{
	remainingTime = time;
}

bool Frozen::LessThan(const Frozen& other)
{
	return remainingTime < other.remainingTime;
}

float Frozen::GetTime() const
{
	return remainingTime;
}

bool Frozen::Advance(const float dt)
{
	return (remainingTime -= dt) <= 0;
}

Frozen::operator bool()
{
	return (0 < remainingTime);
}

DebuffComponent::DebuffComponent(const DamageOverTime& dot)
{
	m_dot = dot;
}

DebuffComponent::DebuffComponent(const Frozen& frozen)
{
	m_frozen = frozen;
}

bool DebuffComponent::Advance()
{
	float dt = GetDeltaTime();
	return (m_frozen.Advance(dt) && m_dot.Advance(dt));
}
