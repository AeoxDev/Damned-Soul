#include "RelicFunctions.h"
// Include MemLib
#include "MemLib\MemLib.hpp"
// Used to contain the different function vectors
#include "MemLib\ML_Map.hpp"
// Used to contain the functions of each type
#include "MemLib\ML_Vector.hpp"

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

ML_Vector<VECTOR_FUNCTION_TYPE> Relics::GetFunctionsOfType(const RELIC_FUNCTION_TYPE& type)
{
	_validateRelicFunctions();
	return (*_RelicFunctions)[type];
}

void Relics::ClearRelicFunctions()
{
	// Clear all functions
	for (size_t i = 0; i < Relics::FUNC_END_OF_TYPES; ++i)
	{
		(*_RelicFunctions)[(Relics::RELIC_FUNCTION_TYPE)i].clear();
	}
}

#include "Relics\DemonBonemarrow.h"
Relics::RelicMetaData Relics::DemonBonemarrow(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Demon Bonemarrow",
		/*Filepath*/	"RelicIcons\\Demon_Bonemarrow.png",
		/*Description*/	"Increases your Strength by 15 when obtained."
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();
		// Call the increase strength function
		DEMON_BONEMARROW::IncreasePlayerStrength(nullptr);
		// Add it to the list of On Obtain functions
		(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(DEMON_BONEMARROW::IncreasePlayerStrength);
	}

	// Return the metadata
	return retVal;
}

#include "Relics\FlameWeapon.h"
Relics::RelicMetaData Relics::FlameWeapon(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Flame Weapon",
		/*Filepath*/	"RelicIcons\\Flame_Weapon.png",
		/*Description*/	"Whenever you hit an enemy with a weapon attack, they take an additional 50% Damage Over Time (2 Seconds)."
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();
		// Add the DoT to the weapon
		(*_RelicFunctions)[FUNC_ON_WEAPON_HIT].push_back(FLAME_WEAPON::PlaceDamageOverTime);
	}

	// Return the metadata
	return retVal;
}

#include "Relics\SoulPower.h"
Relics::RelicMetaData Relics::SoulPower(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Soul Power",
		/*Filepath*/	"RelicIcons\\Soul_Power.png",
		/*Description*/	"You gain a dynamic bonus to your Strength equal to the number of Souls you currently possess"
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();

		// Modify player strength by current souls
		// This effect is NOT added to the "On Obtain" list, since this isn't meant to be a one and done, but rather a setup for a constantly updating bonus
		SOUL_POWER::ModifyPlayerStrengthInitial(nullptr);

		// Add the "On Soul Update" function that takes the delta of souls and modifies Strength by it and adds it to the list
		(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SOUL_POWER::ModifyPlayerStrengthUpdate);
	}

	// Return the metadata
	return retVal;
}

#include "Relics\DemonHeart.h"
Relics::RelicMetaData Relics::DemonHeart(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Demon Heart",
		/*Filepath*/	"RelicIcons\\Demon_Heart.png",
		/*Description*/	"Increases your Maximum Health by 25 when obtained."
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();
		// Call the increase health function
		DEMON_HEART::IncreasePlayerHealth(nullptr);
		// Add it to the list of On Obtain functions
		(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(DEMON_HEART::IncreasePlayerHealth);
	}

	// Return the metadata
	return retVal;
}

#include "Relics\FrostFire.h"
Relics::RelicMetaData Relics::FrostFire(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Frost Fire",
		/*Filepath*/	"RelicIcons\\Frost_Fire.png",
		/*Description*/	"When you are hit by an attack and your remaining Health is less than half your Maximum Health, knock all enemies back and stun them (20s cooldown)"
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();

		//// Call the increase health function
		//DEMON_HEART::IncreasePlayerHealth(nullptr);
		//// Add it to the list of On Obtain functions
		//(*_RelicFunctions)[FUNC_ON_OBTAIN].push_back(DEMON_HEART::IncreasePlayerHealth);
	}

	// Return the metadata
	return retVal;
}

#include "Relics/SoulHealth.h"
Relics::RelicMetaData Relics::SoulHealth(const bool AddRelicFunctions)
{
	RelicMetaData retVal =
	{
		/*Name*/		"Soul Health",
		/*Filepath*/	"RelicIcons\\Soul_Health.png",
		/*Description*/	"You gain a dynamic bonus to your Health equal to the number of Souls you possess."
	};

	// If the relic is to be added, and not just "read", run the functionality
	if (AddRelicFunctions)
	{
		// Make sure the relic function map exists
		_validateRelicFunctions();

		// Modify player Health by current souls
		// This effect is NOT added to the "On Obtain" list, since this isn't meant to be a one and done, but rather a setup for a constantly updating bonus
		SOUL_HEALTH::ModifyPlayerHealthInitial(nullptr);

		// Add the "On Soul Update" function that takes the delta of souls and modifies Health by it and adds it to the list
		(*_RelicFunctions)[FUNC_ON_SOUL_UPDATE].push_back(SOUL_HEALTH::ModifyPlayerHealthUpdate);
	}

	// Return the metadata
	return retVal;
}
