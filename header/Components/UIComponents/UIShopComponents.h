#pragma once
#include "Relics/RelicFunctions.h"

#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

enum shopState
{
	IGNORED = -1,
	AVALIABLE,
	SELECTED,
	BOUGHT,
	LOCKED,

};

struct UIShopButtonComponent
{
	ML_String m_name;
	ML_String m_description;
	uint8_t m_price;

	void Setup(const char* name, const char* desc, uint8_t price)
	{
		m_name = name;
		m_description = desc;
		m_price = price;
	};

	void Release()
	{
		m_name.~ML_String();
		m_description.~ML_String();
	};
};

struct UIShopRerollComponent
{

};

struct UIShopHealComponent
{

};

struct UIShopUpgradeComponent
{

};

struct UIShopPlayerStatsComponent
{

};

struct UIShopTitleImpComponent
{
	ML_String name;

	void Release() { name.~ML_String(); };
};

struct UIShopImpComponent
{
	ML_String name;
	ML_String description;
	uint8_t price;

	void Release()
	{
		name.~ML_String();
		description.~ML_String();
	};
};

struct UIShopRelicComponent
{
	ML_Array<shopState, 2> shopSelections;
	ML_Array<const RelicData*, 2> shopRelics;

	UIShopRelicComponent()
	{
		shopSelections[0] = AVALIABLE;
		shopSelections[1] = AVALIABLE;
	};

	void Release()
	{
		shopSelections.~ML_Array();
		shopRelics.~ML_Array();
	};
};
