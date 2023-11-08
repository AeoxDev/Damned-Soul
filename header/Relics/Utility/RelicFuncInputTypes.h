#pragma once
// For entity
#include "Registry.h"

namespace RelicInput
{
	struct OnStatCalcInput
	{
		EntityID entity;
		void* adressOfStatComonent;
	};

	struct OnHitInput
	{
		EntityID attacker;
		EntityID defender;
	};

	struct OnDamageCalculation
	{
		EntityID attacker = { -1, 0 };
		EntityID defender = { -1, 0 };
		float damage = 0.f; // The "base" damage
		float incMult = 1.f; // The increase multiplier
		float decMult = 1.f; // The decrease multiplier
		float flatAdd = 0.f; // The flat addition
		float flatSub = 0.f; // The flat decrease
		float cap = 0.f; // The maximum damage that can be dealt

		float CollapseDamage();
	};

	struct OnHealthUpdate
	{
		void* adressOfStatComponent;
		float hpDelta;
	};

	struct OnSoulUpdate
	{
		int32_t soulDelta;
	};

	struct OnTimeUpdate
	{
		float timeDelta;
	};
}