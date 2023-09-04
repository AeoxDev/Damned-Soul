#pragma once
#include "PoolPointer.hpp"

namespace MemLib
{
	void createMemoryManager();

	void destroyMemoryManager();

	// Allocate to the memory pools
	PoolPointer<char> palloc(const size_t& size);

	// Free from the memory pools
	bool pfree(const PoolPointer<char>& pp);

	// Push to the memory stack
	void* spush(const size_t& size);

	// Pop from the memory stack
	bool spop();

	// Defrag a single memory pool, cycling through them individually
	void pdefrag();
}