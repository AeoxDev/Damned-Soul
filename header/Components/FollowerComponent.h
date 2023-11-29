#pragma once
#include "EntityID.h"

struct FollowerComponent
{
	EntityID m_target = { -1, true, ENT_PERSIST_LOWEST };

	FollowerComponent(const EntityID& target)
	{
		m_target = target;
	}
};