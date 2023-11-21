#pragma once
#include "MemLib\ML_ComponentMap.hpp"
#include <stdexcept>
#include <algorithm>

#define BIT_64 (sizeof(size_t))
#define PAIR_SIZE (m_tSize + BIT_64)
#define GET_KEY(idx) ((size_t&)(m_data[(idx) * PAIR_SIZE]))
#define GET_KEY_POINTER(idx) (&(m_data[(idx) * PAIR_SIZE]))
#define GET_VALUE_POINTER(idx) (&(m_data[(idx) * PAIR_SIZE + BIT_64]))

// Not used at the moment, but might be useful in the future?
void swapData(void* first, void* second, const size_t pairSize)
{
	// Allocate on stack
	void* temp = MemLib::spush(pairSize);

	// Copy to the temp
	std::memcpy(temp, first, pairSize);

	// Copy second to first
	std::memcpy(first, second, pairSize);

	// Copy temp to second
	std::memcpy(second, temp, pairSize);
}

// Specialized quick sort implementation.
// We know that all maps will begin at size 0, and that only one elment, at most, will ever be removed or added at the time.
// As such, we can safely make the assumption that the middlemost elemnet in terms of value is, probably, the middlemost index.
// Likewise, we know that it will never be possible for one side to have more than half of the elments rounded up plus 1, and can limit space usage according to that.
void sortData(char* data, const uint32_t count, const size_t pairSize)
{
#define GET_KEY_QS(idx) ((size_t&)(data[(idx) * (pairSize)]))
#define GET_KEY_POINTER_QS(idx, data_d) (&(data_d[(idx) * (pairSize)]))

	// If this contains 1 or fewer elements, return without doing anything
	if (count < 2)
		return;

	uint32_t pivot = count / 2;
	// For some reason the pivot is being corrupted and I don't get it...
	size_t& pivotValue = GET_KEY_QS(pivot);
	size_t pivotNonRef = pivotValue;

	// More
	char* more = (char*)MemLib::spush(pairSize * ((count / 2) + 1));
	uint32_t moreCount = 0;

	// More, Pivot
	char* pivotData = (char*)MemLib::spush(pairSize);
	std::memcpy(pivotData, GET_KEY_POINTER_QS(pivot, data), pairSize);

	// More, Pivot, Less
	char* less = (char*)MemLib::spush(pairSize * ((count / 2) + 1));
	uint32_t lessCount = 0;

	for (uint32_t i = 0; i < count; ++i)
	{
		//const size_t compKey = GET_KEY_QS(i);
		if (GET_KEY_QS(i) < pivotValue)
			std::memcpy(GET_KEY_POINTER_QS(lessCount++, less), GET_KEY_POINTER_QS(i, data), pairSize);
		else if (pivotValue < GET_KEY_QS(i))
			std::memcpy(GET_KEY_POINTER_QS(moreCount++, more), GET_KEY_POINTER_QS(i, data), pairSize);
	}

	// Recursive for the lesser partition
	sortData(less, lessCount, pairSize);
	// Recursive for the greater partition
	sortData(more, moreCount, pairSize);

	// Copy less
	std::memcpy(data, less, pairSize * lessCount);
	// More, Pivot
	MemLib::spop();

	// Copy pivot back
	std::memcpy(GET_KEY_POINTER_QS(lessCount, data), pivotData, pairSize);
	// More
	MemLib::spop();

	// Copy more
	std::memcpy(GET_KEY_POINTER_QS(lessCount + 1, data), more, pairSize * moreCount);
	// Stack popped
	MemLib::spop();
}

const bool ML_ComponentMap::initialized() const
{
	return ((m_tSize != 0) && (m_capacity != 0) && (false == m_data.IsNullptr()));
}


const bool ML_ComponentMap::contains(const size_t& key) const
{
	for (uint32_t i = 0; i < m_size; ++i)
	{
		if (key == GET_KEY(i))
			return true;
	}

	return false;
}

const uint32_t& ML_ComponentMap::size() const
{
	return m_size;
}

const void* ML_ComponentMap::data() const
{
	return m_data;
}

// Reserve a new capacity for the map
uint32_t ML_ComponentMap::reserve(const uint32_t& capacity)
{
	if (capacity < m_capacity)
	{
		throw std::invalid_argument("Capacity too small! ML_ComponentMap.reserve() cannot be called to reduce the capacity of the map!");
		std::terminate();
	}
	else if (m_data.IsNullptr())
	{
		m_data = MemLib::palloc(capacity * PAIR_SIZE);
	}
	else
	{
		// Provide a temporary copy of the data
		void* temp = MemLib::spush(m_capacity * PAIR_SIZE);
		std::memcpy(temp, &(*m_data), m_capacity * PAIR_SIZE);

		// Free the old pool pointer and allocate a new one
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(capacity * PAIR_SIZE);

		// Copy the data over to the new location and pop the temp from the stack
		std::memcpy(&(*m_data), temp, m_capacity * PAIR_SIZE);
		MemLib::spop();
	}

	// Inform the new capacity
	return m_capacity = capacity;
};

// Clear the map
void ML_ComponentMap::clear()
{
	m_size = 0;
}

// Push an item into the map and return the adress of that item
void* ML_ComponentMap::emplace(const size_t& key)
{
	for (uint32_t i = 0; i < m_size; ++i)
	{
		const size_t tempKey = GET_KEY(i);
		if (key == tempKey)
		{
			// Return the value if a match was found
			return GET_VALUE_POINTER(i);
		}
	}

	// if the capacity of the map is less than the size of the map, reserve a larger chunk of memory
	if (m_capacity <= m_size)
	{
		reserve(m_capacity * 2 + (m_capacity == 0));
	}

	//for (uint32_t i = 0; i < m_size; ++i)
	//{
	//	if (key == GET_KEY(i))
	//	{
	//		throw std::invalid_argument("Item with key already exists in ML_Map!");
	//		std::terminate();
	//	}
	//}

	// Get adress to set
	GET_KEY(m_size++) = key;
	// Sort the values
	sortData(m_data, m_size, PAIR_SIZE);

	// Return the adress
	return (*this)[key];
};

// Remove an item in the map by key
// Returns the new size of the map
uint32_t ML_ComponentMap::erase(const size_t& key)
{
	for (uint32_t i = 0; i < m_size; ++i)
	{
		if (key == GET_KEY(i))
		{
			// Overwrite
			if (i < (m_size - 1))
				std::memcpy(GET_KEY_POINTER(i), GET_KEY_POINTER(i+1), (m_size - i) * PAIR_SIZE);
			return --m_size;
			// No need to sort, as it should already be sorted before this and it simply shifts one chunk a step over an element that is to be deleted
		}
	}

	throw std::out_of_range("No key with such a value exists in ML_ComponentMap!");
	std::terminate();
};

void* ML_ComponentMap::operator[](const size_t& key)
{
	for (uint32_t i = 0; i < m_size; ++i)
	{
		if (key == GET_KEY(i))
		{
			// Return the value if a match was found
			return GET_VALUE_POINTER(i);
		}
	}

	//// If it doesn't exist, emplace first and return (technically a recursive call)
	//return emplace(key);

	throw std::out_of_range("No key with such a value exists in ML_ComponentMap!");
	std::terminate();
	// To avoid compiler screeching
	return nullptr;
};

ML_ComponentMap& ML_ComponentMap::operator=(const ML_ComponentMap& other)
{
	// First clear
	if (header == HEADER_ID)
	{
		clear();
	}

	m_size = other.m_size;
	m_capacity = other.m_capacity;
	m_tSize = other.m_tSize;

	// Shallow copy, acceptable only because of usage context
	m_data = other.m_data;

	return *this;
};

// Initiate an ML_Map<tKey, tVal> with a number of T objects, can be called as such to emulate normal C++ style coding
// ML_Map<tKey, tVal>() = { args };
ML_ComponentMap::ML_ComponentMap()
{
	m_size = 0;
	// Set capacity
	m_capacity = 0;
	// Set the individual item size
	m_tSize = 0;
};

const ML_ComponentMap& ML_ComponentMap::Initialize(const size_t& sizeofType)
{
	m_size = 0;
	// Set capacity
	m_capacity = 1;
	// Set the individual item size
	m_tSize = (uint32_t)sizeofType;

	// Allocate new to memory pool
	m_data = MemLib::palloc(m_capacity * PAIR_SIZE);

	return *this;
}

ML_ComponentMap::ML_ComponentMap(const ML_ComponentMap& to_copy)
{
	m_size = to_copy.m_size;
	// Set capacity
	m_capacity = to_copy.m_capacity;
	// Set the individual item size
	m_tSize = to_copy.m_tSize;

	// Shallow copy, acceptable only because of the context of how ML_ComponentMap is used
	m_data = to_copy.m_data;
}

ML_ComponentMap::~ML_ComponentMap()
{
	clear();
	// No need to free since the component maps are (more or less) persistent
	//MemLib::pfree(m_data);
}