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
#include "Relics\Offensive\FlameWeapon.h"
#include "Relics\Offensive\SoulPower.h"
#include "Relics\Offensive\LifeSteal.h"
#include "Relics\Offensive\AdvancedFighting.h"
	/*Defense*/
#include "Relics\Defensive\Hearts\DemonHeart.h"
#include "Relics\Defensive\Hearts\CorruptedHeart.h"
#include "Relics\Defensive\Hearts\MoltenHeart.h"
#include "Relics\Defensive\Hearts\MummifiedHeart.h"
#include "Relics\Defensive\FrostFire.h"
#include "Relics\Defensive\SoulHealth.h"
#include "Relics\Defensive\DemonSkin.h"
#include "Relics\Defensive\SpikedSkin.h"
	/*Gadget*/
#include "Relics\Gadget\SpeedyLittleDevil.h"
#include "Relics\Gadget\LightningGod.h"
#include "Relics\Gadget\Reckless.h"
#include "Relics\Gadget\SoulSpeed.h"
// End of include all relics

#include "Relics\Utility\ML_RelicArray.h"
#include "MemLib\ML_Vector.hpp"

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

	std::srand(time(NULL));
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
	/*Filepath*/	"RelicIcons\\DefaultStone_Relic.png",
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
			RelicData(
				/*Name*/		"Demon Bonemarrow",
				/*Filepath*/	"RelicIcons\\Demon_Bonemarrow.png",
				/*Description*/	"Increases your Damage by 15 when obtained.",
				/*Price*/		10,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	DEMON_BONEMARROW::Initialize
			),
			RelicData(
				/*Name*/		"Flame Weapon",
				/*Filepath*/	"RelicIcons\\Flame_Weapon.png",
				/*Description*/	"Whenever you hit an enemy with a weapon attack, they take *Burn* for an additional 65% Damage over 1.75 Seconds.",
				/*Price*/		3,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	FLAME_WEAPON::Initialize
			),
			RelicData(
				/*Name*/		"Soul Power",
				/*Filepath*/	"RelicIcons\\Soul_Power.png",
				/*Description*/	"You gain a dynamic bonus to your Damage equal to the number of Souls you currently possess",
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	SOUL_POWER::Initialize
			),
			RelicData(
				/*Name*/		"Life Steal",
				/*Filepath*/	"RelicIcons\\Life_Steal.png",
				/*Description*/	"You heal for 15% of the damage you deal",
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	LIFE_STEAL::Initialize
			),
			RelicData(
				/*Name*/		"Advanced Fighting",
				/*Filepath*/	"RelicIcons\\Advanced_Fighting.png",
				/*Description*/	"Your attacks gain a 35% chance to critically hit, dealing double their normal damage",
				/*Price*/		5,
				/*Type*/		RELIC_OFFENSE,
				/*Function*/	ADVANCED_FIGHTING::Initialize
			),
			RelicData(
				/*Name*/		"Demon Heart",
				/*Filepath*/	"RelicIcons\\Demon_Heart.png",
				/*Description*/	"Increases your Maximum Health by 25",
				/*Price*/		6,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	DEMON_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Corrupted Heart",
				/*Filepath*/	"RelicIcons\\Corrupted_Heart.png",
				/*Description*/	"Increases your Maximum Health by 40",
				/*Price*/		9,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	CORRUPTED_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Molten Heart",
				/*Filepath*/	"RelicIcons\\Molten_Heart.png",
				/*Description*/	"Increases your Maximum Health by 20, and restores 20 Health when first obtained.",
				/*Price*/		7,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	MOLTEN_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Mummified Heart",
				/*Filepath*/	"RelicIcons\\Mummified_Heart.png",
				/*Description*/	"Increases your Maximum Health by 15",
				/*Price*/		3,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	MUMMIFIED_HEART::Initialize
			),
			RelicData(
				/*Name*/		"Frost Fire",
				/*Filepath*/	"RelicIcons\\Frost_Fire.png",
				/*Description*/	"Once per level when you are hit by an attack and your remaining Health is less than half your Maximum Health, knock all enemies back and permanently slow them by 20%",
				/*Price*/		3,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	FROST_FIRE::Initialize
			),
			RelicData(
				/*Name*/		"Soul Health",
				/*Filepath*/	"RelicIcons\\Soul_Health.png",
				/*Description*/	"You gain a dynamic bonus to your Health equal to the number of Souls you possess.",
				/*Price*/		5,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	SOUL_HEALTH::Initialize
			),
			RelicData(
				/*Name*/		"Demon Skin",
				/*Filepath*/	"RelicIcons\\Demon_Skin.png",
				/*Description*/	"Reduces the damage you take by 20%",
				/*Price*/		5,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	DEMON_SKIN::Initialize
			),
			RelicData(
				/*Name*/		"Spiked Skin",
				/*Filepath*/	"RelicIcons\\Spiked_Skin.png",
				/*Description*/	"Whenever an enemy strikes you, it takes 15% of the damage it would deal (before reduction) as irresistable damage",
				/*Price*/		5,
				/*Type*/		RELIC_DEFENSE,
				/*Function*/	SPIKED_SKIN::Initialize
			),
			RelicData(
				/*Name*/		"Speedy Little Devil",
				/*Filepath*/	"RelicIcons\\Speedy_Little_Devil.png",
				/*Description*/	"Increases your Movespeed by 4 when obtained",
				/*Price*/		3,
				/*Type*/		RELIC_GADGET,
				/*Function*/	SPEEDY_LITTLE_DEVIL::Initialize
			),
			RelicData(
				/*Name*/		"Lightning God",
				/*Filepath*/	"RelicIcons\\Lightning_God.png",
				/*Description*/	"A bolt of lightning strikes a random enemy every few seconds, dealing massive irresistable damage",
				/*Price*/		0,
				/*Type*/		RELIC_GADGET,
				/*Function*/	LIGHTNING_GOD::Initialize
			),
			RelicData(
				/*Name*/		"Reckless",
				/*Filepath*/	"RelicIcons\\Reckless.png",
				/*Description*/	"You deal 50% more damage, but also take double damage",
				/*Price*/		10,
				/*Type*/		RELIC_GADGET,
				/*Function*/	RECKLESS::Initialize
			),
			RelicData(
				/*Name*/		"Soul Speed",
				/*Filepath*/	"RelicIcons\\Soul_Speed.png",
				/*Description*/	"You gain a bonus to your speed equal to your souls",
				/*Price*/		5,
				/*Type*/		RELIC_GADGET,
				/*Function*/	SOUL_SPEED::Initialize
			),

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

const RelicData* Relics::PickRandomRelic(const RELIC_TYPE& type)
{
	_validateMasterRelicList();

	if (type == RELIC_UNTYPED && (*RemainingRelicList).size())
	{
		uint32_t randomlySelected = std::rand() % (*RemainingRelicList).size();
		const RelicData* retVal = (*RemainingRelicList)[randomlySelected];
		(*RemainingRelicList).erase(randomlySelected);
		return retVal;
	}

	return &DefaultRelicRock;
}

const RelicData* Relics::PickNamedRelic(const char* name)
{
	return &DefaultRelicRock;
}

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
