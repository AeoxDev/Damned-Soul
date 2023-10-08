#pragma once
#include "MemLib\ML_Map.hpp"
#include "Registry.h"

#define CREATE_ENTITY_MAP_entities ML_Map<int, EntityID> entities;

#define ADD_TO_entities_IF_NOT_INCLUDED(idx)\
if (false == entities.contains(idx.index))	\
	entities.emplace(idx.index, idx);		\


uint16_t DestroyEntities(const ML_Map<int, EntityID>&entities);