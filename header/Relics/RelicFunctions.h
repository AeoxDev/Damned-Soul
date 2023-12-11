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

		// Called when calculating the player's specific bonus stats
		// arguments: foo(EntityID, PlayerComopnent*)
		FUNC_ON_PLAYER_STAT_CALC,

		// Called when damage is calculated
		//arguments: foo(AttackerEntity, DefenderEntity, damageBase, damageMultInc, damageMultDec, damageAdd, damageSub)
		FUNC_ON_DAMAGE_CALC,

		// Called when an entity is dealt damage (before damage is applied
		// arguments: foo(AttackerEntity, DefenderEntity, damageBase, damageMultInc, damageMultDec, damageAdd, damageSub)
		FUNC_ON_DAMAGE_APPLY,

		// Called when damaged (after damage is applied)
		// arguments: foo(TargetStats, hpDelta)
		FUNC_ON_DAMAGE_TAKEN,

		// Called when healed (after healing is applied)
		// arguments: foo(TargetStats, hpDelta)
		FUNC_ON_HEALING_TAKEN,

		// Called when the player's number of souls is updated
		// arguments: foo(soulDelta)
		FUNC_ON_SOUL_UPDATE,

		// Called when a frame is being calculared
		// arguments: foo(timeDelta)
		FUNC_ON_FRAME_UPDATE,

		// Call when a new level is loaded
		// arguments: foo(void)
		FUNC_ON_LEVEL_SWITCH,

		// Called when entity dashes
		//arguments: foo(AttackerEntity, DefenderEntity, damageBase, damageMultInc, damageMultDec, damageAdd, damageSub)
		FUNC_ON_DASH,

		// Called when prices are calculated
		//arguments: foo(cost_all, cost_relics, cost_heal, cost_reroll, upgrade_cost)
		FUNC_ON_PRICE_CALC,

		// Called when the player initiates an attack
		//arguments: foo(void)
		FUNC_ON_NOT_ATTACKING,

		// Used to iterate, not to give functionality to relics
		FUNC_END_OF_TYPES
	};

	enum RELIC_TYPE
	{
		// Default value, picks a fully random relic in PickRandomRelics
		RELIC_UNTYPED = 0x0,
		// Offense relics, picks a weighted offense relic in PickRandomRelics
		RELIC_OFFENSE = 0x1,
		// Defense relics, picks a weighted offense relic in PickRandomRelics
		RELIC_DEFENSE = 0x2,
		// Gadget relics, picks a weighted offense relic in PickRandomRelics
		RELIC_GADGET = 0x4,
		// Relic of any type, differs from RELIC_UNTYPED in that it fetches weighted relics via PickRandomRelics
		RELIC_ANY = RELIC_OFFENSE | RELIC_DEFENSE | RELIC_GADGET
	};

	// Get all active relic functions of a given type
	// Can be called at any place at any time and looped over, as the functions it contains get all data themselves for themselves
	ML_Vector<VECTOR_FUNCTION_TYPE> GetFunctionsOfType(const RELIC_FUNCTION_TYPE& type);

	// Call upon Game Over to clear all relics.
	// Curretnly not being used since there is no real "Game Over" function, just regular unload.
	void ResetRelics();

	// Gets a random relic of the selected type
	// Untyped returns completely ranodm, while specific types returns that type
	// Specified types returns relics weighted by their costs (giving priority to cheap relics early on)
	const RelicData* PickRandomRelic(const RELIC_TYPE& type);

	//// Get a specific relic by its name
	//const RelicData* PickNamedRelic(const char* name);

	// Puts a relic back into the pool of relics
	bool PutBackRelic(const RelicData* relic);
}