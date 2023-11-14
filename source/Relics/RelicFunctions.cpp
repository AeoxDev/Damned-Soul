#include "Relics\RelicFunctions.h"
#include "Relics\Utility\RelicInternalHelper.h"
// Include MemLib
#include "MemLib\MemLib.hpp"
// Used to contain the different function vectors
#include "MemLib\ML_Map.hpp"
// Used to contain the functions of each type
#include "MemLib\ML_Vector.hpp"


// Include all relics
	/*Offense*/
#include "Relics\Offensive\DemonBonemarrow.h"
#include "Relics\Offensive\LifeSteal.h"
#include "Relics\Offensive\AdvancedFighting.h"
#include "Relics\Offensive\SoulPower.h"
#include "Relics\Offensive\FlameWeapon.h"
//#include "Relics\Offensive\ExplodingWeapon.h"
#include "Relics\Offensive\DashAttack.h"
#include "Relics\Offensive\AdrenalineRush.h"
#include "Relics\Offensive\ThrillSeeker.h"
//#include "Relics\Offensive\PowerStrike.h"


	/*Defense*/
#include "Relics\Defensive\Hearts\DemonHeart.h"
#include "Relics\Defensive\Hearts\CorruptedHeart.h"
#include "Relics\Defensive\Hearts\MoltenHeart.h"
#include "Relics\Defensive\Hearts\MummifiedHeart.h"
#include "Relics\Defensive\FrostFire.h"
#include "Relics\Defensive\SoulHealth.h"
#include "Relics\Defensive\DemonSkin.h"
#include "Relics\Defensive\SpikedSkin.h"
#include "Relics\Defensive\UndeadFortitude.h"
#include "Relics\Defensive\PainMirror.h"
#include "Relics\Defensive\GoldenDuck.h"
#include "Relics\Defensive\BackShield.h"
#include "Relics\Defensive\SecondWind.h"
#include "Relics\Defensive\LastStand.h"
	/*Gadget*/
#include "Relics\Gadget\SpeedyLittleDevil.h"
#include "Relics\Gadget\LightningGod.h"
#include "Relics\Gadget\Reckless.h"
#include "Relics\Gadget\SoulSpeed.h"
// End of include all relics

#include "Relics\Utility\ML_RelicArray.h"
#include "MemLib\ML_Vector.hpp"

// For active level
#include "States\StateManager.h"

#include <random>
#include <time.h>

ML_Map<RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>* _RelicFunctions = nullptr;

// Initialize _RelicFunctions if it isn't already
void _validateRelicFunctions()
{
	if (_RelicFunctions)
		return;

	// Persistent push
	_RelicFunctions = (ML_Map<RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>*)MemLib::spush(sizeof(ML_Map<RELIC_FUNCTION_TYPE, ML_Vector<VECTOR_FUNCTION_TYPE>>));
	_RelicFunctions->Initialize();

	for (size_t i = 0; i < FUNC_END_OF_TYPES; ++i)
	{
		// Emplace the type of function
		_RelicFunctions->emplace((RELIC_FUNCTION_TYPE)i, ML_Vector<VECTOR_FUNCTION_TYPE>());
	}

	std::srand((unsigned)time(NULL));
};

// Master list of relics, used to keep track of all relics that exist
ML_RelicArray MasterRelicList;

// The list of relics that remain this run
ML_Vector<const RelicData*>* RemainingRelicList = nullptr;
//#define REMAINING_RELICS (*RemainingRelicList)

// The "function" the default stone does
void doNothing(void* nothing) { return; }
RelicData DefaultRelicRock(
	/*Name*/		"Useless Rock",
	/*Filepath*/	"RelicIcons\\DefaultStone_Relic",
	/*Description*/	"Somewhat shiny? It is advertised to do SOMETHING but it seems to be just a pebble...",
	/*Price*/		1,
	/*Type*/		RELIC_UNTYPED,
	/*Function*/	doNothing
);

void _validateMasterRelicList()
{
	if (0 == MasterRelicList.m_size)
	{
		ML_RelicArray MasterInitializer {

			//OFFENSIVE RELICS
			RelicData(
				/*Name*/		"Demon Bonemarrow",
				/*Filepath*/	"RelicIcons\\Demon_Bonemarrow",
				/*Description*/	DEMON_BONEMARROW::Description(),
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	DEMON_BONEMARROW::Initialize
			),
			RelicData(
				/*Name*/		"Flame Weapon",
				/*Filepath*/	"RelicIcons\\Flame_Weapon",
				/*Description*/	FLAME_WEAPON::Description(),//"Whenever you hit an enemy with a weapon attack, they take *Burn* for an additional 65% Damage over 1.75 Seconds.",
				/*Price*/		3,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	FLAME_WEAPON::Initialize
			),
			RelicData(
				/*Name*/		"Soul Power",
				/*Filepath*/	"RelicIcons\\Soul_Power",
				/*Description*/	"You gain 1 Strength for every soul in your posession",
				/*Price*/		10,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	SOUL_POWER::Initialize
			),
			RelicData(
				/*Name*/		"Life Steal",
				/*Filepath*/	"RelicIcons\\Life_Steal",
				/*Description*/	"You heal for 15% of the damage you deal",
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	LIFE_STEAL::Initialize
			),
			RelicData(
				/*Name*/		"Advanced Fighting",
				/*Filepath*/	"RelicIcons\\Advanced_Fighting",
				/*Description*/	"Your attacks gain a 35% chance to critically hit, dealing double their normal damage",
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	ADVANCED_FIGHTING::Initialize
			),
			RelicData(
				/*Name*/		"Dash Attack",
				/*Filepath*/	"RelicIcons\\Dash_Attack",
				/*Description*/	"Deals 50% of your damage when dashing through an enemy",
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	DASH_ATTACK::Initialize
			),
			RelicData(
				/*Name*/		"Adrenaline Rush",
				/*Filepath*/	"RelicIcons\\Adrenaline_Rush",
				/*Description*/	"You attack twice as fast for 1 second after being hit",
				/*Price*/		10,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	ADRENALINE_RUSH::Initialize
			),
			RelicData(
				/*Name*/		"Thrill Seeker",
				/*Filepath*/	"RelicIcons\\Thrill_Seeker",
				/*Description*/	"For every % hp lost, gain % damage",
				/*Price*/		10,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	THRILL_SEEKER::Initialize
			),
			//ENDOF: OFFENSIVE RELICS


			//DEFENSIVE RELICS
			RelicData(
				/*Name*/		"Demon Heart",
				/*Filepath*/	"RelicIcons\\Demon_Heart",
				/*Description*/	"Increases your Maximum Health by 25",
				/*Price*/		5,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	DEMON_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Corrupted Heart",
				/*Filepath*/	"RelicIcons\\Corrupted_Heart",
				/*Description*/	"Increases your Maximum Health by 40",
				/*Price*/		8,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	CORRUPTED_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Molten Heart",
				/*Filepath*/	"RelicIcons\\Molten_Heart",
				/*Description*/	"Increases your Maximum Health by 20, and restores 20 Health when first obtained",
				/*Price*/		6,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	MOLTEN_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Mummified Heart",
				/*Filepath*/	"RelicIcons\\Mummified_Heart",
				/*Description*/	"Increases your Maximum Health by 15",
				/*Price*/		3,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	MUMMIFIED_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Frost Fire",
				/*Filepath*/	"RelicIcons\\Frost_Fire",
				/*Description*/	"Every few seconds, send out a shockwave that knocks back [[and damages?]] nearby enemies",
				/*Price*/		3,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	FROST_FIRE::Initialize
			),
			RelicData(
				/*Name*/		"Soul Health",
				/*Filepath*/	"RelicIcons\\Soul_Health",
				/*Description*/	"You gain a dynamic bonus to your Health equal to the number of Souls you possess",
				/*Price*/		10,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	SOUL_HEALTH::Initialize
			),
			RelicData(
				/*Name*/		"Demon Skin",
				/*Filepath*/	"RelicIcons\\Demon_Skin",
				/*Description*/	"Reduces the damage you take by 20%",
				/*Price*/		5,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	DEMON_SKIN::Initialize
			),
			RelicData(
				/*Name*/		"Spiked Skin",
				/*Filepath*/	"RelicIcons\\Spiked_Skin",
				/*Description*/	"Whenever an enemy strikes you, it takes 100% of the damage it would deal (before reduction) as irresistable damage",
				/*Price*/		3,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	SPIKED_SKIN::Initialize
			),
			RelicData(
				/*Name*/		"Undead Fortitude",
				/*Filepath*/	"RelicIcons\\Undead_Fortitude",
				/*Description*/	"Reduces the damage you take by 10%-40%, based on how low your current health is",
				/*Price*/		6,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	UNDEAD_FORTITUDE::Initialize
			),
			RelicData(
				/*Name*/		"Pain Mirror",
				/*Filepath*/	"RelicIcons\\Empty_Relic",
				/*Description*/	"Whenever an enemy strikes you, it takes 150% of the damage it would deal (before reduction) as irresistable damage",
				/*Price*/		8,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	PAIN_MIRROR::Initialize
			),
			RelicData(
				/*Name*/		"Golden Duck",
				/*Filepath*/	"RelicIcons\\Golden_Duck",
				/*Description*/	"You are immune to up to 3 seconds of exposure to damaging hazards. Recharges while not in use",
				/*Price*/		8,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	GOLDEN_DUCK::Initialize
			),
			RelicData(
				/*Name*/		"Back Shield",
				/*Filepath*/	"RelicIcons\\Shield",
				/*Description*/	"Up to once every 2 seconds, you nullify the next attack when struck from behind",
				/*Price*/		8,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	BACK_SHIELD::Initialize
			),
			RelicData(
				/*Name*/		"Second Wind",
				/*Filepath*/	"RelicIcons\\Second_Wind",
				/*Description*/	"The first time that you would die while holding this relic, your remaining hit points are instead set to 25% of your hit point maximum",
				/*Price*/		10,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	SECOND_WIND::Initialize
			),
			RelicData(
				/*Name*/		"Last Stand",
				/*Filepath*/	"RelicIcons\\Last_Stand",
				/*Description*/	"While below 50% health, you block 50% of damage taken from attacks up to once every 2 seconds",
				/*Price*/		6,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	LAST_STAND::Initialize
			),
			//ENDOF: DEFENSIVE RELICS


			//GADGET RELICS
			RelicData(
				/*Name*/		"Speedy Little Devil",
				/*Filepath*/	"RelicIcons\\Speedy_Little_Devil",
				/*Description*/	"Increases your Movespeed by 4 when obtained",
				/*Price*/		3,
				/*Type*/		RELIC_GADGET,
				/*Function*/	SPEEDY_LITTLE_DEVIL::Initialize
			),
			RelicData(
				/*Name*/		"Lightning God",
				/*Filepath*/	"RelicIcons\\Lightning_God",
				/*Description*/	"Every three seconds, a random enemy is marked for the next three seconds. If a marked enemy is dealt damage by you, it takes an additional 10 damage",
				/*Price*/		10,
				/*Type*/		RELIC_GADGET,
				/*Function*/	LIGHTNING_GOD::Initialize
			),
			RelicData(
				/*Name*/		"Reckless",
				/*Filepath*/	"RelicIcons\\Reckless",
				/*Description*/	"You deal 50% more damage, but also take double damage",
				/*Price*/		10,
				/*Type*/		RELIC_GADGET,
				/*Function*/	RECKLESS::Initialize
			),
			RelicData(
				/*Name*/		"Soul Speed",
				/*Filepath*/	"RelicIcons\\Soul_Speed",
				/*Description*/	"You gain a bonus to your speed equal to your souls",
				/*Price*/		5,
				/*Type*/		RELIC_GADGET,
				/*Function*/	SOUL_SPEED::Initialize
			),
			//ENDOF: GADGET RELICS
		};
		// Copy over
		std::memcpy(&MasterRelicList, &MasterInitializer, sizeof(ML_RelicArray));

		RemainingRelicList = (ML_Vector<const RelicData*>*)MemLib::spush(sizeof(ML_Vector<const RelicData*>));
		(*RemainingRelicList).Initialize();

		// "Copy" it all over to the current list
		for (unsigned int i = 0; i < MasterRelicList.m_size; ++i)
		{
			RemainingRelicList->push_back(MasterRelicList[i]);
		}
	}
}

ML_Vector<VECTOR_FUNCTION_TYPE> Relics::GetFunctionsOfType(const RELIC_FUNCTION_TYPE& type)
{
	_validateRelicFunctions();
	return (*_RelicFunctions)[type];
}

void Relics::ResetRelics()
{
	_validateRelicFunctions();
	_validateMasterRelicList();

	// Clear all functions
	for (size_t i = 0; i < FUNC_END_OF_TYPES; ++i)
	{
		if (_RelicFunctions)
		{
			(*_RelicFunctions)[(RELIC_FUNCTION_TYPE)i].clear();
		}
	}

	// Clear the remaining relic list
	(*RemainingRelicList).clear();
	// Refill the remaining relic list
	for (unsigned int i = 0; i < MasterRelicList.m_size; ++i)
	{
		RemainingRelicList->push_back(MasterRelicList[i]);
	}
}

#define CLAMP_SUB(a, b) (a < b ? 0 : a - b)

const RelicData* Relics::PickRandomRelic(const RELIC_TYPE& type)
{
	_validateMasterRelicList();

	// The return value
	// Default to useless rock!
	const RelicData* retVal = &DefaultRelicRock;

	// If a specific type was requested and there are remaining relics
	if (type && (*RemainingRelicList).size())
	{
		// The potential relics matching the criteria
		ML_Map<uint32_t, const RelicData*> possibleSelection;
		// The current total weight
		uint32_t currentTotalWeight = 0;

		for (uint32_t i = 0; i < (*RemainingRelicList).size() && type /*should also be typed*/; /*++i*/)
		{
			const RelicData* currentRelic = (*RemainingRelicList)[i];

			// If the relic is a match for the selected type, add it to the possible selections
			if (currentRelic->m_typeFlag & type) {
				// Set weight inversely by price and add level (cheaper relics more common early on)
				// After a fairly large number of levels, this weight system will give everything an equal chance
				currentTotalWeight += CLAMP_SUB(5, CLAMP_SUB(currentRelic->m_price, stateManager.activeLevel)) + stateManager.activeLevel;
				// Emplace with the combined weights of previous selection possibilities
				possibleSelection.emplace(currentTotalWeight, currentRelic);
				// Erase current, no need to increment
				(*RemainingRelicList).erase(i);
			}
			// None picked, increment
			else {
				++i;
			}
		}

		// Randomly selected index from the possible selection
		uint32_t randomlySelected = 1 + (std::rand() % currentTotalWeight);
		// -1 means none have been selected yet
		int64_t randomlySelectedKey = -1;

		// Set return value and remove from the possible selections
		for (auto& [key, val] : possibleSelection)
		{
			// If the key is less than or equal to the the selection, set values
			// Also set if nothing has been chosen yet
			if (key <= randomlySelected  || -1 == randomlySelectedKey)
			{
				retVal = val;
				randomlySelectedKey = key;
			}
			// Else, break out of the loop, the chosen one has been found!
			else
			{
				break;
			}
		}
		possibleSelection.erase(randomlySelectedKey);

		// Return the relics that weren't picked
		for (auto& [key, val] : possibleSelection) {
			(*RemainingRelicList).push_back(val);
		}
	}
	// Otherwise, just select a random one (given one exists)
	else if ((*RemainingRelicList).size())
	{
		uint32_t randomlySelected = std::rand() % (*RemainingRelicList).size();
		retVal = (*RemainingRelicList)[randomlySelected];
		(*RemainingRelicList).erase(randomlySelected);
	}


	return retVal;
}

//const RelicData* Relics::PickNamedRelic(const char* name)
//{
//	return &DefaultRelicRock;
//}

bool Relics::PutBackRelic(const RelicData* relic)
{
	// If the list already contains the relic, the "relic" is nullptr, or the relic is the useless rock, do nothing and return false
	if (nullptr == relic || 
		std::find((*RemainingRelicList).begin(), (*RemainingRelicList).end(), relic) != (*RemainingRelicList).end() ||
		&DefaultRelicRock == relic)
		return false;
	// Also check if its already in there somehow maybe I dunno I'm not your mom
	RemainingRelicList->push_back(relic);
	return true;
}
