#pragma once
struct DamageOverTimeComponent
{
	float remainingTime = -1.f;
	float damagePerSecond = 0.f;

	bool LessThan(const DamageOverTimeComponent& other)
	{
		return (remainingTime * damagePerSecond) < (other.remainingTime * other.damagePerSecond);
	}
};