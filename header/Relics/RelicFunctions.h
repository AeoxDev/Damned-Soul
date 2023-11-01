#pragma once
#include "MemLib\ML_Vector.hpp"
#include "RelicData.h"

namespace Relics
{
	// All functions are to be void functions with void* arguments.
	#define VECTOR_FUNCTION_TYPE void(*)(void*)

	enum RELIC_FUNCTION_TYPE
	{
		// Called when the relic is purchased or otherwise obtained
		// arguments: foo(void)
		FUNC_ON_OBTAIN,

		// Called when calculating an entity's bonus stats
		// arguments: foo(EntityID, StatComponent*)
		FUNC_ON_STAT_CALC,

		// Called when an enemy is hit by a weapon attack
		// arguments: foo(PlayerEntity, EnemyEntity)
		FUNC_ON_WEAPON_HIT,

		// Called when the player's health is modified, such as when struck by an attack or healed
		// arguments: foo(hpDelta)
		FUNC_ON_HEALTH_MODIFIED,

		// Called when the player's number of souls is updated
		// arguments: foo(soulDelta)
		FUNC_ON_SOUL_UPDATE,

		// Called when a frame is being calculared
		// arguments: foo(timeDelta)
		FUNC_ON_FRAME_UPDATE,

		// Call when a new level is loaded
		// arguments: foo(void)
		FUNC_ON_LEVEL_SWITCH,

		// Used to iterate, not to give functionality to relics
		FUNC_END_OF_TYPES
	};

	enum RELIC_TYPE
	{
		RELIC_UNTYPED = 0x0,
		RELIC_OFFENSE = 0x1,
		RELIC_DEFENSE = 0x2,
		RELIC_GADGET = 0x4
	};

	// Get all active relic functions of a given type
	// Can be called at any place at any time and looped over, as the functions it contains get all data themselves for themselves
	ML_Vector<VECTOR_FUNCTION_TYPE> GetFunctionsOfType(const RELIC_FUNCTION_TYPE& type);

	// Call upon Game Over to clear all relics.
	// Curretnly not being used since there is no real "Game Over" function, just regular unload.
	void ResetRelics();

	// Gets a random relic of the selected type
	// Untyped returns completely ranodm, while specific types returns that type
	const RelicData* PickRandomRelic(const RELIC_TYPE& type);

	// Get a specific relic by its name
	const RelicData* PickNamedRelic(const char* name);

	// Puts a relic back into the pool of relics
	bool PutBackRelic(const RelicData* relic);
}