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
		void* adressOfStatComponent;
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