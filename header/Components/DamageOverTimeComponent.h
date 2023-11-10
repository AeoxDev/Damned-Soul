#pragma once
struct DamageOverTimeComponent
{
private:
	float remainingTime = -1.f;
	float damagePerSecond = 0.f;

public:
	DamageOverTimeComponent(const float DpS, const float time);

	// Check if one DoT component has less damage remaining than another one
	bool LessThan(const DamageOverTimeComponent& other);

	// Get remaining time (cannot exceed current delta time)
	float GetTime() const;

	// Get the DPS
	float GetDPS() const;

	// Advance the time of the DoT
	// If the remaining time is zero, return true (such that an if statement can remove the component)
	bool Advance();

};