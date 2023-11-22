#pragma once
#include "MemLib\MemLib.hpp"
#include <cinttypes>

#define HEADER_ID 0b0101010101010101010

struct ML_ComponentMap
{
private:
	int header = HEADER_ID;
	// Pool pointer to internal data
	PoolPointer<char> m_data;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_size;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_capacity;
	// size of the internal type
	uint16_t m_tSize;

public:
	const bool initialized() const;

	const bool contains(const size_t& key) const;

	const uint32_t& size() const;

	const void* data() const;

	// Reserve a new capacity for the map
	uint32_t reserve(const uint32_t& capacity);

	// Clear the map
	void clear();

	// Push an item into the map and return the adress of that item
	void* emplace(const size_t& key);

	// Remove an item in the map by key
	// Returns the new size of the map
	uint32_t erase(const size_t& key);

	void* operator[](const size_t& key);

	ML_ComponentMap& operator=(const ML_ComponentMap& other);

	// Initiate an ML_Map<tKey, tVal> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Map<tKey, tVal>() = { args };
	ML_ComponentMap();

	void /*const ML_ComponentMap&*/ Initialize(const size_t& sizeofType);

	ML_ComponentMap(const ML_ComponentMap& to_copy);

	~ML_ComponentMap();
};