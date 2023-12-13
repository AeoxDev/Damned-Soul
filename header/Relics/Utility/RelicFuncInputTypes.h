#pragma once
// For entity
#include "Registry.h"

namespace RelicInput
{
	namespace DMG
	{
		enum DAMAGE_TYPE_AND_SOURCE
		{
			UNDEFINED = 0x0,
			INSTANT = 0x1,
			DOT = 0x2,
			ENEMY = 0x4,
			HAZARD = 0x8,
			REFLECT = 0x10,
			RANGED = 0x20,
			DASH = 0x40, // IMPORTANT, EYE DASH IS NOT A DASH DAMAGE EFFECT!
			INSTANT_ENEMY = INSTANT | ENEMY,
			DOT_ENEMY = DOT | ENEMY,
			INSTANT_HAZARD = INSTANT | HAZARD,
			DOT_HAZARD = DOT | HAZARD
		};
	}

	struct OnStatCalcInput
	{
		EntityID entity;
		void* adressOfStatComonent;
	};

	struct OnStatCalcInputPlayer
	{
		EntityID entity;
		void* adressOfPlayerComonent;
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
		DMG::DAMAGE_TYPE_AND_SOURCE typeSource = DMG::UNDEFINED; // The type and source of damage

		float CollapseDamage();
		float CollapseNoCap();
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

	struct OnPriceCalculation
	{
		float everythingCostMult = 1.f;
		float relicCostMult = 1.f;
		float healCostMult = 1.f;
		float rerollCostMult = 1.f;
		float upgradeCost = 1.f;

		enum PRICE_TYPES
		{
			RELIC = 0x1,
			HEAL = 0x2,
			REROLL = 0x4,
			UPGRADE = 0x8
		};

		int32_t GetCostOf(const int32_t initial, const PRICE_TYPES type);
	};
}