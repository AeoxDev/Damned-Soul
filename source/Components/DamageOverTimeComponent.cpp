#include "DamageOverTimeComponent.h"
#include "DeltaTime.h"

DamageOverTimeComponent::DamageOverTimeComponent(const float DpS, const float time)
{
	damagePerSecond = DpS;
	remainingTime = time;
}

bool DamageOverTimeComponent::LessThan(const DamageOverTimeComponent& other)
{
	return (remainingTime * damagePerSecond) < (other.remainingTime * other.damagePerSecond);
}

float DamageOverTimeComponent::GetTime() const
{
	float dt = GetDeltaTime();
	return dt < remainingTime ? dt : remainingTime;
}

float DamageOverTimeComponent::GetDPS() const
{
	return damagePerSecond;
}

bool DamageOverTimeComponent::Advance()
{
	return (remainingTime -= GetDeltaTime()) <= 0;
}
