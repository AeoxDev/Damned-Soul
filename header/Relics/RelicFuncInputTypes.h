#pragma once
// For entity
#include "Registry.h"

namespace RelicInput
{
	struct OnHitInput
	{
		EntityID player;
		EntityID enemy;
	};

	struct OnHealthUpdate
	{
		float hpDelta;
	};

	struct OnSoulUpdate
	{
		size_t soulDelta;
	};
}