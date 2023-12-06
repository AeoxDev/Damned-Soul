#pragma once
#include "MemLib/ML_String.hpp"

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
	bool freebie = true;
};

struct UIShopUpgradeComponent
{

};