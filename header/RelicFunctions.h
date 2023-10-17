#pragma once
#include "MemLib\ML_Vector.hpp"

/*
	The idea is that each relic will have a set of functions that represents what it performs.
	These are either run when the relic is bought, or placed into a vector of similar functions to be looped over.
	The exact structure is not entirely certain at the moment
*/

namespace Relics
{
	// All functions are to be void functions with void* arguments.
	#define VECTOR_FUNCTION_TYPE void(*)(void*)

	enum RELIC_FUNCTION_TYPE
	{
		FUNC_ON_OBTAIN, // Called when the relic is purchased or otherwise obtained, and never again
		FUNC_ON_WEAPON_HIT, // Called when an enemy is hit by a weapon attack
		FUNC_END_OF_TYPES // Used to iterate, not give functionality to relics
	};

	// Get all active relic functions of a given type
	// Can be called at any place at any time and looped over, as the functions it contains get all data themselves for themselves
	ML_Vector<void(*)(void*)> GetFunctionsOfType(const RELIC_FUNCTION_TYPE& type);

	// Call upon Game Over to clear all relics.
	// Curretnly not being used since there is no real "Game Over" function, just regular unload.
	void ClearRelicFunctions();

	struct RelicMetaData
	{
		// The name of the relic
		char relicName[32] = "_NAME";
		// The filepath to the relics' icon
		char filePath[96] = "_FILEPATH";
		// The description of the relic
		char description[384] = "_DESCRIPTION";
	};

	// Get the metadata of the relic "Demon Bonemarrow".
	// Setting 'AddRelicFunctions = true' calls and/or adds the relic's functions
	RelicMetaData DemonBonemarrow(const bool AddRelicFunctions = false);

	// Get the metadata of the relic "Flame Weapon".
	// Setting 'AddRelicFunctions = true' calls and/or adds the relic's functions
	RelicMetaData FlameWeapon(const bool AddRelicFunctions = false);

	// Get the metadata of the relic "Demon Heart".
	// Setting 'AddRelicFunctions = true' calls and/or adds the relic's functions
	RelicMetaData DemonHeart(const bool AddRelicFunctions = false);
}