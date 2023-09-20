#pragma once
#include "MemLib\MemLib.hpp"
#include <cinttypes>
#include <stdexcept>

template<typename T>
struct ML_Vector
{
private:
	// Pool pointer to internal data
	PoolPointer<T> m_data;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_size = 0;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_capacity = 0;
	// size of the internal type
	uint16_t m_tSize = 0;

public:
	T* begin() const
	{
		return &(m_data[0]);
	}

	T* end() const
	{
		return &(m_data[m_size]);
	}

	const uint32_t& size() const
	{
		return m_size;
	}

	const PoolPointer<T>& data() const
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

		// Provide a temporary copy of the data
		T* temp = (T*)MemLib::spush(m_capacity * m_tSize);
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

	// Clear the vector
	void clear()
	{
		m_size = 0;
		m_capacity = 0;
		MemLib::pfree(m_data);
	}

	// Push an item into the back of the vector, returns the index of that item
	uint32_t push_back(const T& item)
	{
		// if the capacity of the vector is less than the size of the vector, reserve a larger chunk of memory
		if (m_capacity <= m_size + 1)
		{
			// Branchlessly add 4 to the capacity if it is zero
			reserve(m_capacity * 2 + (m_capacity == 0) * 4);
		}

		// Set data at location
		std::memcpy(&m_data[m_size], &item, m_tSize);
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
		std::memcpy(&(m_data[idx]), &(m_data[idx + 1]), (m_size - idx) * m_tSize);

		return --m_size;
	};

	// Pop and return the back most item of the vector
	T pop_back()
	{
		if (m_size <= 0)
		{
			throw std::out_of_range("Out of range exception for ML_Vector at pop_back(), vector already empty!");
			std::terminate();
		}

		// No need to actually remove the data in any "real" way, just mark it as empty
		return m_data[--m_size];
	};

	T& operator*()
	{
		return (*m_data);
	};

	T& operator[](const uint32_t& idx)
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
		// Update size and capacity
		m_size = other.m_size;
		m_tSize = other.m_tSize;
		m_capacity = other.m_capacity;
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(other.m_capacity * other.m_tSize);

		std::memcpy(m_data, other.m_data, m_tSize * m_size);
		return *this;
	};

	// Initiate an ML_Vector<T> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Vector<T>() = { args };
	ML_Vector()
	{
		// Set capacity
		m_capacity = 4;
		// Set the individual item size
		m_tSize = sizeof(T);

		// Allocate to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);
	};

	ML_Vector& Initialize()
	{
		// Set capacity
		m_capacity = 4;
		// Set the individual item size
		m_tSize = sizeof(T);

		// Allocate to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);
	};

	// Initiate an ML_Vector<T> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Vector<T>() = { args };
	template<typename... Types>
	ML_Vector(Types... args)
	{
		// Set capacity
		m_capacity = sizeof...(args);
		// Set the individual item size
		m_tSize = sizeof(T);

		// Allocate to memory pool
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_capacity * m_tSize);

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
		MemLib::pfree(m_data);
	};
};