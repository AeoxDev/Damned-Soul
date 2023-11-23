#pragma once
#include "MemLib\MemLib.hpp"
#include <cinttypes>
#include <stdexcept>
//#include <vector>
#define HEADER_ID 0b0101010101010101010

template<typename _T>
struct ML_Vector
{
private:
	int header = HEADER_ID;
	// Pool pointer to internal data
	PoolPointer<_T> m_data;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_size;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_capacity;
	// size of the internal type
	uint16_t m_tSize;

	//std::vector<int> test;

public:
	_T* begin() const
	{
		return &(m_data[0]);
	}

	_T* end() const
	{
		return &(m_data[m_size]);
	}

	const uint32_t& size() const
	{
		return m_size;
	}

	const PoolPointer<_T>& data() const
	{
		return m_data;
	}

	// Reserve a new capacity for the vector
	uint32_t reserve(const uint32_t& capacity)
	{
		if (capacity < m_capacity)
		{
			throw std::invalid_argument("Capacity too small! ML_Vector.reserve() cannot be called to reduce the capacity of the vector!");
			std::terminate();
		}
		else if (m_data.IsNullptr())
		{
			m_data = MemLib::palloc((capacity * (size_t)m_tSize));
		}
		else
		{
			// Provide a temporary copy of the data
			_T* temp = (_T*)MemLib::spush((m_capacity * (size_t)m_tSize));
			std::memcpy(temp, &(*m_data), (m_capacity * (size_t)m_tSize));

			// Free the old pool pointer and allocate a new one
			MemLib::pfree(m_data);
			m_data = MemLib::palloc((capacity * (size_t)m_tSize));

			// Copy the data over to the new location and pop the temp from the stack
			std::memcpy(&(*m_data), temp, (m_capacity * (size_t)m_tSize));
			MemLib::spop();
		}

		// Inform the new capacity
		return m_capacity = capacity;
	};

	// Clear the vector
	void clear()
	{
		for (uint32_t i = 0; i < m_size; ++i)
			m_data[i].~_T();
		m_size = 0;
	}

	uint32_t emplace(const uint32_t position, const _T& item)
	{
		// if the capacity of the vector is less than the size of the vector, reserve a larger chunk of memory
		if (m_capacity <= m_size + 1)
		{
			// Branchlessly add 4 to the capacity if it is zero
			reserve(m_capacity * 2 + (m_capacity == 0));
		}

		// The data size to copy
		uint32_t copySize = (m_size - position) * m_tSize;

		//Create a temp variable with all elements before the selected position
		void* temp = MemLib::spush(copySize);

		std::memcpy(temp, &(m_data[position]), copySize);

		// Copy onto the position
		new (&m_data[position]) _T(item);
		// Increment size
		++m_size;

		// Copy the "tail" back onto the vector
		std::memcpy(&(m_data[position+1]), temp, copySize);
		// Pop the temp
		MemLib::spop();

		// Return the new size of the vector
		return m_size;
	}

	// Push an item into the back of the vector, returns the index of that item
	uint32_t push_back(const _T& item)
	{
		// if the capacity of the vector is less than the size of the vector, reserve a larger chunk of memory
		if (m_capacity <= m_size + 1)
		{
			// Branchlessly add 4 to the capacity if it is zero
			reserve(m_capacity * 2 + (m_capacity == 0));
		}

		// Set data at location
		new(&m_data[m_size]) _T(item);
		//std::memcpy(&m_data[m_size], &item, m_tSize);
		//m_data[m_size] = item;

		// Return the index of the newly pushed object
		return m_size++;
	};

	// Push an item into the back of the vector, returns the index of that item
	template<typename... Args>
	uint32_t push_back(Args... args)
	{
		// if the capacity of the vector is less than the size of the vector, reserve a larger chunk of memory
		if (m_capacity <= m_size + 1)
		{
			// Branchlessly add 4 to the capacity if it is zero
			reserve(m_capacity * 2 + (m_capacity == 0));
		}

		// Set data at location
		m_data[m_size] = *(new(&m_data[m_size]) _T({args...}));
		//std::memcpy(&m_data[m_size], &item, m_tSize);
		//m_data[m_size] = item;

		// Return the index of the newly pushed object
		return m_size++;
	};

	// Remove an item in the vecotr by index
	// This operation is much slower than pop_back, and should not be used if pop_back could simply be used instead
	uint32_t erase(const uint32_t& idx)
	{
		if (idx < 0 || m_size <= idx)
		{
			throw std::out_of_range("Index provided for ML_Vector is out of range!");
			std::terminate();
		}

		// Overwrite
		if (idx < (m_size - 1))
			std::memcpy(&(m_data[idx]), &(m_data[idx + 1]), (m_size - idx) * m_tSize);

		return --m_size;
	};

	// Pop and return the back most item of the vector
	_T pop_back()
	{
		if (m_size <= 0)
		{
			throw std::out_of_range("Out of range exception for ML_Vector at pop_back(), vector already empty!");
			std::terminate();
		}

		// No need to actually remove the data in any "real" way, just mark it as empty
		return m_data[--m_size];
	};

	_T& operator*()
	{
		return (*m_data);
	};

	_T& operator[](const uint32_t& idx)
	{
		if (m_size <= idx)
		{
			throw std::out_of_range("Index provided for ML_Vector is out of range!");
			std::terminate();
		}

		return m_data[idx];
	};

	ML_Vector& operator=(const ML_Vector& other)
	{
		// First clear
		if (header == HEADER_ID)
		{
			clear();
		}

		// Update size and capacity
		m_size = other.m_size;
		m_tSize = other.m_tSize;
		m_capacity = other.m_capacity;
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(other.m_capacity * other.m_tSize);

		for (uint32_t i = 0; i < m_size; ++i)
			new(&m_data[i]) _T(other.m_data[i]);
		//std::memcpy(m_data, other.m_data, m_tSize * m_size);
		return *this;
	};

	// Initiate an ML_Vector<T> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Vector<T>() = { args };
	ML_Vector()
	{
		m_size = 0;
		// Set capacity
		m_capacity = 4;
		// Set the individual item size
		m_tSize = sizeof(_T);

		// Allocate to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc((m_capacity * (size_t)m_tSize));
	};

	void Initialize()
	{
		m_size = 0;
		// Set capacity
		m_capacity = 4;
		// Set the individual item size
		m_tSize = sizeof(_T);

		// Allocate to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc((m_capacity * (size_t)m_tSize));
	};

	ML_Vector(const ML_Vector& to_copy)
	{
		m_size = to_copy.m_size;
		// Set the capacity
		m_capacity = to_copy.m_capacity;
		// Set the individual item size
		m_tSize = to_copy.m_tSize;

		// Free the old memory
		MemLib::pfree(m_data);
		m_data = MemLib::palloc((m_capacity * (size_t)m_tSize));
		// Set items

		for (uint32_t i = 0; i < m_size; ++i)
			new(&m_data[i]) _T(to_copy.m_data[i]);
	}

	// Initiate an ML_Vector<T> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Vector<T>() = { args };
	template<typename... Types>
	ML_Vector(Types... args)
	{
		m_size = 0;
		// Set capacity
		m_capacity = sizeof...(args);
		// Set the individual item size
		m_tSize = sizeof(_T);

		// Allocate to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc((m_capacity * (size_t)m_tSize));

		// Set items
		for (auto item : { args... } )
		{
			/*T test = item;*/
			push_back(item);
			//ZeroMemory(item, sizeof(item));
		}
	};

	~ML_Vector()
	{
		clear();
		m_capacity = 0;
		MemLib::pfree(m_data);
	};
};