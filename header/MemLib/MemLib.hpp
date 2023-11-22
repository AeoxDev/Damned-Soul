#pragma once
#include "PoolPointer.hpp"

namespace MemLib
{
	void createMemoryManager();

	void destroyMemoryManager();

	// Allocate to the memory pools
	PoolPointer<char> palloc(const size_t& size);

	bool BACKGROUND_PFREE_DONT_TOUCH(const PoolPointer<char>& pp);

	// Free from the memory pools
	template<typename T>
	bool pfree(PoolPointer<T>& pp)
	{
		bool retVal = BACKGROUND_PFREE_DONT_TOUCH(pp);
#ifdef _DEBUG
		pp.m_ac = 0;
#endif // _DEBUG
		pp.m_pp = nullptr;
		pp.m_ps = 255;
		pp.m_pi = 255;
		return retVal;
	}

	// Push to the memory stack
	void* spush(const size_t& size);

	// Pop from the memory stack
	bool spop();

	// Defrag a single memory pool, cycling through them individually
	void pdefrag();
}