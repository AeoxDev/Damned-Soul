#include "DebuffComponent.h"
#include "ModelComponent.h"
#include "DeltaTime.h"
#include <cinttypes>
#include <cmath>

DamageOverTime::DamageOverTime(const float DpS, const float time)
{
	damagePerSecond = DpS;
	remainingTime = time;
}

void DamageOverTime::AlterModelColor(void* model, const DOT_TYPE& type)
{
	// Both model components have the relevant data at the same place, typecast between them shouldn't matter in this case
	ModelSkeletonComponent* cast = (ModelSkeletonComponent*)model;
	float power = std::sqrt(WeightedPower());
	if (POISON == type)
	{
		// Multiplicative
		cast->shared.bcmR_temp *= (1.03f + (.02f * power));
		cast->shared.bcmG_temp /= (1.05f + (.04f * power));
		cast->shared.bcmB_temp *= (1.05f + (.04f * power));
		// Additive
		cast->shared.bcaR_temp += .03f;
		cast->shared.bcaG_temp -= .06f + (.06f * power);
		cast->shared.bcaB_temp += .05f;
	}
	else if (BURN == type)
	{
		// Multiplicative
		cast->shared.bcmR_temp *= (1.1f + (.05f * power));
		cast->shared.bcmG_temp *= (1.04f + (.02f * power));
		cast->shared.bcmB_temp /= (1.1f + (.1f * power));
		// Additive
		cast->shared.bcaR_temp += .12f;
		cast->shared.bcaG_temp += .03f;
		cast->shared.bcmB_temp -= .1 + (.08f * power);
	}
}

#define E (2.71828f)
#define PRIMTIVE_POINT_NINE_POW_X(x) (-9.49122f*std::powf(0.9f,(x)))
float DamageOverTime::WeightedPower() const
{
	float P_X = PRIMTIVE_POINT_NINE_POW_X(remainingTime);
	float P_0 = PRIMTIVE_POINT_NINE_POW_X(0.0f);
	float weightedDuration = P_X - P_0;
	return weightedDuration * damagePerSecond;
}

bool DamageOverTime::LessThan(const DamageOverTime& other)
{
	return WeightedPower() < other.WeightedPower();
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

DebuffComponent::DebuffComponent(const DamageOverTime::DOT_TYPE& type, const DamageOverTime& dot)
{
	m_dots[type] = dot;
}

DebuffComponent::DebuffComponent(const Frozen& frozen)
{
	m_frozen = frozen;
}

bool DebuffComponent::Advance()
{
	float dt = GetDeltaTime();
	bool retVal = m_frozen.Advance(dt);
	for (uint8_t i = 0; i < DamageOverTime::END_OF_DOTS; ++i)
	{
		retVal &= m_dots[i].Advance(dt);
	}
	return retVal;
}
