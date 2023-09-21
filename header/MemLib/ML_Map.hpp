#pragma once
#include "MemLib\MemLib.hpp"
#include <cinttypes>
#include <stdexcept>

template<typename _tKey, typename _tVal>
struct ML_Map
{
private:
	// Pool pointer to internal data
	PoolPointer<std::pair<_tKey, _tVal>> m_data;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_size;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_capacity;
	// size of the internal type
	uint16_t m_tSize;

public:
	std::pair<_tKey, _tVal>* begin() const
	{
		return &(m_data[0]);
	}

	std::pair<_tKey, _tVal>* end() const
	{
		return &(m_data[m_size]);
	}

	const uint32_t& size() const
	{
		return m_size;
	}

	const PoolPointer<std::pair<_tKey, _tVal>>& data() const
	{
		return m_data;
	}

	// Reserve a new capacity for the map
	uint32_t reserve(const uint32_t& capacity)
	{
		if (capacity < m_capacity)
		{
			throw std::invalid_argument("Capacity too small! ML_Map.reserve() cannot be called to reduce the capacity of the map!");
			std::terminate();
		}

		// Provide a temporary copy of the data
		std::pair<_tKey, _tVal>* temp = (std::pair<_tKey, _tVal>*)MemLib::spush(m_capacity * m_tSize);
		std::memcpy(temp, &(*m_data), m_capacity * m_tSize);

		// Free the old pool pointer and allocate a new one
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(capacity * m_tSize);

		// Copy the data over to the new location and pop the temp from the stack
		std::memcpy(&(*m_data), temp, m_capacity * m_tSize);
		MemLib::spop();

		// Inform the new capacity
		return m_capacity = capacity;
	};

	// Clear the map
	void clear()
	{
		for (uint32_t i = 0; i < m_size; ++i)
		{
			m_data[i].first.~_tKey();
			m_data[i].second.~_tVal();
			m_data[i].~pair();
		}
			
		m_size = 0;
	}

	// Push an item into the back of the map, returns the index of that item
	uint32_t emplace(const _tKey& key, const _tVal& val)
	{
		// if the capacity of the map is less than the size of the map, reserve a larger chunk of memory
		if (m_capacity < m_size)
		{
			reserve(m_capacity * 2 + (m_capacity == 0));
		}

		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (key == m_data[i].first)
			{
				throw std::invalid_argument("Item with key already exists in ML_Map!");
				std::terminate();
			}
		}

		// Set data at location
		//std::pair<tKey, tVal> temp = { key, val };
		//std::memcpy(&m_data[m_size], &temp, m_tSize);
		new(&m_data[m_size]) std::pair<_tKey,_tVal>(key, val);

		// Return the index of the newly pushed object
		return m_size++;
	};

	// Remove an item in the map by key
	// Returns the new size of the map
	uint32_t erase(const _tKey& key)
	{
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (key == m_data[i].first)
			{
				// Overwrite
				std::memcpy(&(m_data[i]), &(m_data[i + 1]), (m_size - i) * m_tSize);
				return --m_size;
			}
		}

		throw std::out_of_range("No key with such a value exists in ML_Map!");
		std::terminate();
	};

	_tVal& operator[](const _tKey& key)
	{
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (key == m_data[i].first)
			{
				// Return the value if a match was found
				return m_data[i].second;
			}
		}

		throw std::out_of_range("No key with such a value exists in ML_Map!");
		std::terminate();

		// To avoid compiler screeching
		return m_data[0].second;
	};

	ML_Map& operator=(const ML_Map& other)
	{
		// First clear
		clear();

		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_tSize = other.m_tSize;

		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);

		for (uint32_t i = 0; i < m_size; ++i)
			new(&m_data[i]) std::pair<_tKey,_tVal>(other.m_data[i].first, other.m_data[i].second);
		//std::memcpy(m_data, other.m_data, m_tSize * m_size);
		return *this;
	}

	// Initiate an ML_Map<tKey, tVal> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Map<tKey, tVal>() = { args };
	ML_Map()
	{
		m_size = 0;
		// Set capacity
		m_capacity = 4;
		// Set the individual item size
		m_tSize = sizeof(std::pair<_tKey, _tVal>);

		// Allocate new to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);
	};

	const ML_Map& Initialize()
	{
		m_size = 0;
		// Set capacity
		m_capacity = 4;
		// Set the individual item size
		m_tSize = sizeof(std::pair<_tKey, _tVal>);

		// Allocate new to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);

		return *this;
	}

	ML_Map(const ML_Map& to_copy)
	{
		m_size = to_copy.m_size;
		// Set capacity
		m_capacity = to_copy.m_capacity;
		// Set the individual item size
		m_tSize = to_copy.m_tSize;

		// Free the old memory
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);
		// Set items

		for (uint32_t i = 0; i < m_size; ++i)
			new(&m_data[i]) std::pair<_tKey, _tVal>(to_copy.m_data[i].first, to_copy.m_data[i].second);
	}

	// Initiate an ML_Map<tKey, tVal> with a number of pairs, can be called as such to emulate normal C++ style coding
	// ML_Map<tKey, tVal>() = { {kay, val}, ... };
	template<typename... Pairs>
	ML_Map(Pairs... pairs)
	{
		m_size = 0;
		// Set capacity
		m_capacity = sizeof...(pairs);
		// Set the individual item size
		m_tSize = sizeof(std::pair<_tKey, _tVal>);

		// Free the old memory
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);

		// Set items
		for (const std::pair<_tKey, _tVal>& pair : { pairs... })
		{
			emplace(pair.first, pair.second);
		}
	};

	~ML_Map()
	{
		clear();
		MemLib::pfree(m_data);
	}
};