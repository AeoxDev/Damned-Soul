#pragma once
/*
		THIS FILE IS MEANT TO ONLY BE INCLUDED IN RELIC IMPLEMENTATION CPP FILES!
		DO NOT INCLUDE THIS OUTSIDE ANY DIRECT RELIC FUNCTIONALITY
		DO NOT ALTER IT
		DO NOT USE IT
		I WILL FIND OUT WHERE YOU LIVE
		/Erika
*/

#include "MemLib\ML_String.hpp"
#include "MemLib\ML_Map.hpp"
#include "Relics\RelicFunctions.h"
#include <stdio.h>

using namespace Relics;

// A map using relic types as keys
// Contains all active relic functions
extern ML_Map<RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>* _RelicFunctions;

// Validate relics
void _validateRelicFunctions();

#define PERCENT(f) (long int((f)*100))

// Calls MarkAsModified, UpdateBonusHealth, and StealthilyModifyHealth
// These need to be called for relics like the hearts, which raise max hp and current hp simultaneously
// Everything is contained with a mini scope
#define RELIC_RAISE_CURRENT_MAX_HP(owner, delta)						\
{																		\
	StatComponent* stats = registry.GetComponent<StatComponent>(owner);	\
	stats->MarkAsModified();											\
	stats->UpdateBonusHealth(delta);									\
	stats->StealthilyModifyHealth(delta);								\
}																							