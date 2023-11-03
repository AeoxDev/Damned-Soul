#pragma once
#include <cinttypes>
#include "MemLib\MemLib.hpp"
#include "Relics\RelicData.h"
#include <cstring>

struct ML_RelicArray
{
	RelicData* m_stackData = nullptr;
	uint8_t m_size = 0;
	
	ML_RelicArray() = default;

	template<typename... Relics>
	ML_RelicArray(const Relics&... relics)
	{
		// Set capacity
		m_size = sizeof...(relics);
		// Set the individual item size

		// Allocate to memory pool
		m_stackData = (RelicData*)MemLib::spush(sizeof(RelicData) * m_size);

		// Set items
		uint8_t i = 0;
		for (auto item : { relics... })
		{
			std::memcpy(&m_stackData[i++], &item, sizeof(RelicData));
		}
	}

	const RelicData* operator[](const uint8_t idx) const
	{
		return m_stackData + idx;
	}
};