#include "RelicFunctions.h"
// Include MemLib
#include "MemLib\MemLib.hpp"
// Used to contain the different function vectors
#include "MemLib\ML_Map.hpp"
// Used to contain the functions of each type
#include "MemLib\ML_Vector.hpp"

// All functions are to be void functions with void* arguments.
#define VECTOR_FUNCTION_TYPE void(*)(void*)
// A map using relic types as keys
ML_Map<Relics::RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>* _RelicFunctions = nullptr;

// Initialize _RelicFunctions if it isn't already
void _validateRelicFunctions()
{
	if (_RelicFunctions)
		return;

	// Persistent push
	_RelicFunctions = (ML_Map<Relics::RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>*)MemLib::spush(sizeof(ML_Map<Relics::RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>));
	_RelicFunctions->Initialize();

	for (size_t i = 0; i < Relics::FUNC_END_OF_TYPES; ++i)
	{
		// Emplace the type of function
		_RelicFunctions->emplace((Relics::RELIC_FUNCTION_TYPE)i, ML_Vector<VECTOR_FUNCTION_TYPE>());
	}
};

void* Relics::GetFunctionsOfType(const size_t& count, const RELIC_FUNCTION_TYPE& type)
{
	return (*_RelicFunctions)[type].begin();
}

void Relics::ClearRelicFunctions()
{
	// Clear all functions
	for (size_t i = 0; i < Relics::FUNC_END_OF_TYPES; ++i)
	{
		(*_RelicFunctions)[(Relics::RELIC_FUNCTION_TYPE)i].clear();
	}
}

// Fix Demon Bonemarrow
#include "Relics\DemonBonemarrow.h"
Relics::RelicMetaData Relics::DemonBonemarrow(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Demon Bonemarrow",
		/*Filepath*/	"TempRelic1.png",
		/*Description*/	"Raises your maximum hit points by 25 when obtained."
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();
		// Call the increase health function
		DEMON_BONEMARROW::IncreasePlayerHealth(nullptr);
		// Add it to the list of On Obtain functions
		(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(DEMON_BONEMARROW::IncreasePlayerHealth);
	}

	// Return the metadata
	return retVal;
}