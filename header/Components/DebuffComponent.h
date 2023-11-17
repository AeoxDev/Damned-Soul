#pragma once
struct DamageOverTime
{
private:
	float remainingTime = -1.f;
	float damagePerSecond = 0.f;

public:
	DamageOverTime() = default;
	DamageOverTime(const float DpS, const float time);

	// Check if one DoT component has less damage remaining than another one
	bool LessThan(const DamageOverTime& other);

	// Get remaining time (cannot exceed current delta time)
	float GetTime() const;

	// Get the DPS
	float GetDPS() const;

	// Advance the time of the DoT
	// If the remaining time is zero, return true (such that an if statement can remove the component)
	bool Advance(const float dt);

	operator bool();
};

struct Frozen
{
private:
	float remainingTime = -1.f;

public:
	Frozen() = default;
	Frozen(const float time);

	// Check if one DoT component has less damage remaining than another one
	bool LessThan(const Frozen& other);

	// Get remaining time (cannot exceed current delta time)
	float GetTime() const;

	// Advance the time of the Freeze
	// If the remaining time is zero, return true (such that an if statement can remove the component)
	bool Advance(const float dt);

	operator bool();
};

struct DebuffComponent
{
	DamageOverTime m_dot;
	Frozen m_frozen;

	DebuffComponent(const DamageOverTime& dot);
	DebuffComponent(const Frozen& frozen);

	bool Advance();
};